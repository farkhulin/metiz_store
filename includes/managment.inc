<?php
/**
 * Metiz Store module Order Managment functions.
 */

function metiz_store_form($form, &$form_state) {
  $ordersTotalCounter = db_query("SELECT COUNT(*) FROM orders")->fetchField();
  $ordersNewCounter = db_query("SELECT COUNT(*) FROM orders WHERE status=0")->fetchField();
  $ordersProcessCounter = db_query("SELECT COUNT(*) FROM orders WHERE status=1")->fetchField();
  $ordersDoneCounter = db_query("SELECT COUNT(*) FROM orders WHERE status=2")->fetchField();
  $ordersTotlaCost = db_query("SELECT SUM(cost) total FROM orders_data")->fetchField();
  $ordersTotlaCost = metiz_store_price_formatter($ordersTotlaCost);

  $statisticsHTML = '
    <div class="store-statistics">
      <div class="left">
        Всего заказов: '.$ordersTotalCounter.'</br>
        Общая сумма: '.$ordersTotlaCost.'
      </div>
      <div class="right">
        Новых заказов: '.$ordersNewCounter.'</br>
        В работе заказов: '.$ordersProcessCounter.'</br>
        Выполненных заказов: '.$ordersDoneCounter.'
      </div>
    </div>
  ';
  $output['info'] = array(
    '#markup' => '<h2>Управление заказами:</h2>'.$statisticsHTML,
  );

  $values = array(
    'all' => t('-Любой-'),
    '0' => t('Новый'),
    '1' => t('В работе'),
    '2' => t('Выполнен')
  );

  $output['order_status'] = array(
    '#title' => t('Статус заказа:'),
    '#type' => 'select',
    '#options' => $values,
    '#default_value' => $_SESSION['metiz_store_order_status'],
  );

  $output['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Применить'),
  );

  $header = array(
    array('data' => '№ Заказа', 'field' => 'oid', 'sort' => 'DESC'),
    array('data' => 'UID', 'field' => 'uid', 'sort' => 'ASC'),
    array('data' => 'Имя', 'field' => 'name', 'sort' => 'ASC'),
    array('data' => 'Email'),
    array('data' => 'Телефон'),
    array('data' => 'Адрес доставки'),
    array('data' => 'Кол-во товаров'),
    array('data' => 'Итого'),
    array('data' => 'Дата/Время', 'field' => 'oid', 'sort' => 'ASC'),
    array('data' => 'Статус', 'field' => 'status', 'sort' => 'ASC'),
  );

  if (trim(variable_get('metiz_store_orders_number')) != 'all') {
    $query = db_select('orders', 'orders')->extend('PagerDefault')->extend('TableSort');
  } else {
    $query = db_select('orders', 'orders')->extend('TableSort');
  }
  $condition = array(0,1,2);

  if (isset($form_state['values']['order_status'])) {
    $_SESSION['metiz_store_order_status'] = $form_state['values']['order_status'];
  }

  if (isset($_SESSION['metiz_store_order_status']) && $_SESSION['metiz_store_order_status'] != 'all') {
    $condition = array($_SESSION['metiz_store_order_status']);
  }

  if (trim(variable_get('metiz_store_orders_number')) == 'all') {
    $query->condition('status', $condition)
        ->fields('orders', array('oid', 'uid', 'name', 'mail', 'phone', 'address', 'status'))
        ->orderByHeader($header);
        // ->orderBy('oid', 'DESC');
  } else {
    $query->condition('status', $condition)
        ->fields('orders', array('oid', 'uid', 'name', 'mail', 'phone', 'address', 'status'))
        ->orderByHeader($header)
        // ->orderBy('oid', 'DESC')
        ->limit(trim(variable_get('metiz_store_orders_number')));
  }

  $results = $query->execute();

  $orderActions = '
    <div class="actions">
      <span class="new">Новый</span>
      <span class="in-process">В работе</span>
      <span class="done">Выполнен</span>
      <span class="delete">Удалить</span>
      <span class="cancel">Отмена</span>
    </div>
  ';

  $rows = array();
  foreach ($results as $row) {
    $ordersItems = db_query("SELECT * FROM orders_data WHERE oid=".$row->oid)->fetchAll();
    $itemCounter = 0;
    $cost = 0;
    $itemsDetail = '<table style="display:none;"><tr><th>Название товара</th><th>Конфигурация</th><th>Цена</th></tr><tbody>';
    foreach ($ordersItems as $delta => $itemData) {
      $cost = $cost + $itemData->cost;
      $itemCounter++;
      $itemCostFormatted = metiz_store_price_formatter($itemData->cost);
      $productTitle = db_query("SELECT title FROM node WHERE nid=".$itemData->pid)->fetchField();
      $itemsDetail .= '<tr><td>'.$productTitle.'</td><td>'.nl2br($itemData->description).'</td><td>'.$itemCostFormatted.'</td></tr>';
    }
    $itemsDetail .= '</tbody></table>';
    $cost = metiz_store_price_formatter($cost);
    $orderDateTime = str_split($row->oid, 2);
    $orderDateTimeString = $orderDateTime[0].$orderDateTime[1].'.'.$orderDateTime[2].'.'.$orderDateTime[3].' '.$orderDateTime[4].':'.$orderDateTime[5].':'.$orderDateTime[6];
    $orderStatus = '';
    $orderStatusClass = '';
    if ($row->status == 0) {
      $orderStatus = 'Новый';
      $orderStatusClass = 'new';
    }
    if ($row->status == 1) {
      $orderStatus = 'В работе';
      $orderStatusClass = 'in-process';
    }
    if ($row->status == 2) {
      $orderStatus = 'Выполнен';
      $orderStatusClass = 'done';
    }

    $data = array(
      $row->oid,
      $row->uid,
      $row->name,
      $row->mail,
      $row->phone,
      $row->address,
      $itemCounter,
      $cost,
      $orderDateTimeString,
      array(
        'data' => '<i></i><span class="status-text">'.$orderStatus.'</span>'.$orderActions,
        'class' => array('status'),
      ),
    );
    $rows[] = array(
      'data' => $data,
      'class' => array($orderStatusClass),
      'id' => array($row->oid),
    );
    $rows[] = array(
      array(
        'data' => $itemsDetail.'<span class="order-details">Показать детали заказа</span>',
        'colspan' => count($header),
      ),
    );
  }

  $output['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('Заказов с таким статусом пока нет!'),
    '#multiple' => TRUE,
    '#attributes' => array(
      'class' => array('orders-table'),
    ),
  );

  if (trim(variable_get('metiz_store_orders_number')) != 'all') {
    $output['pager'] = array(
      '#theme' => 'pager',
    );
  }

  return $output;
}

function metiz_store_form_validate($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function metiz_store_form_submit($form, &$form_state) {
  // code...
}