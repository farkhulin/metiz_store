<?php
/**
 * Metiz Store module Blocks.
 */

function _metiz_store_cart_block() {
  $cartCounter = count($_SESSION['cart']);
  $cartTotal = $_SESSION['cart_cost'];
  $quantityText = '';
  if ($cartCounter > 0) {
    $quantityText = $cartCounter.' товаров на '.metiz_store_price_formatter($cartTotal);
  } else {
    $quantityText = 'Корзина пуста';
  }
  $output = '
    <div id="cart_box">
      <div class="cart_tag">
        <img src="/images/cart.png" class="cart-tag-img">
        Корзина
      </div>
      <span class="quantity">'.$quantityText.'</span>
      <a class="cart_link red-link" href="/shop/cart">Оформить заказ</a>
    </div>
  ';

  return $output;
}

function _metiz_store_news_block() {

	$newsArray = db_query("select * from `field_data_field_is_news` fdf left join  node  n on fdf.entity_id=n.nid where 1 order by  n.`created` limit 3");
  
  $linkItems = '';
	foreach ($newsArray as $news) {
    $linkItems .= '<li class="news-item"><span class="news-date">'.date('d.m.Y',$news->created).'</span><p class="news-abstract">'.$news->title.'</p><a class="news-more-link" href="/node/'.$news->nid.'">подробнее</a></li> ';
  }
  $output = '
    <div id="news_box" class="sidebar-box">
      <ul class="side-news-box">
        '.$linkItems.'
      </ul>
    </div>
  ';

  return $output;
}

function metiz_store_user_orders() {
  global $user;

  $header = array('#', '№ Заказа', 'Кол-во товаров', 'Итого', 'Дата/Время', 'Статус');

  $query = db_select('orders', 'orders')->extend('PagerDefault');
  $query->condition('uid', $user->uid)
        ->fields('orders', array('oid', 'status'))
        ->orderBy('oid', 'DESC')
        ->limit(10);

  $results = $query->execute();
  
  $queryTotalCost = db_select('orders', 'orders')
        ->fields('orders', array('oid', 'status'))
        ->condition('uid', $user->uid)
        ->orderBy('oid', 'DESC')
        ->execute()
        ->fetchAll();

  $totalCost = 0;
  foreach ($queryTotalCost as $row) {
    $ordersItems = db_query("SELECT * FROM orders_data WHERE oid=".$row->oid)->fetchAll();
    $cost = 0;
    foreach ($ordersItems as $delta => $itemData) {
      $cost = $cost + $itemData->cost;
    }
    $totalCost = $totalCost + $cost;
  }

  $rows = array();
  $rowCounter = $_GET['page'] * 10;
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

    $rowCounter++;
    $data = array(
      $rowCounter,
      $row->oid,
      $itemCounter,
      $cost,
      $orderDateTimeString,
      array(
        'data' => '<i></i><span class="status-text">'.$orderStatus.'</span>',
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
    '#empty' => t('Вы пока не сделали ни одной покупки.'),
    '#attributes' => array(
      'class' => array('user-orders-table'),
    ),
  );

  $output['pager'] = array(
    '#theme' => 'pager',
  );

  $totalCost = metiz_store_price_formatter($totalCost);

  $output['info'] = array(
    '#markup' => '<h3>'.t('Общая сумма заказов').': '.$totalCost.'</h3>',
  );

  return $output;
}
