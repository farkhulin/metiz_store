<?php
/**
 * Metiz Store module Cart functions.
 */

function metiz_store_order_cart() {

  if (count($_SESSION['cart']) != 0) {
    foreach ($_SESSION['cart'] as $key => $value) {
      if ($value['nid'] == '') {
        $n = node_load($value);
      } else {
        $n = node_load($value['nid']);
      }
      $img = explode('/', $n->field_photo['und'][0]['uri']);
      $output .= '<div class="cart-row"><div class="cart-col-1 cart-col"><img src="/sites/default/files/chemical/'.$img[3].'" width="205" height="205" alt=""/><h4 class="cat-item-title">'.$n->title.'</h4></div><div class="cart-col-2 cart-col">';
      if ($_SESSION['cart'][$key]['add_in_cart_nocalc'] == 1) {
        foreach ($_SESSION['cart'][$key] as $field => $data){
          if ($field != 'nid' && $field != 'add_in_cart_nocalc' && $field != 'hidden_sum' && $field != 'table') {
            $parse = explode('|', $data);
            $output .= '<span class="cat-item-dimentions">'.$parse[0].': '.$parse[1].'</span>';
          }
        }
      }
      if ($value['diametr'] != '' && $value['length'] != ''):
        $output .= '<span class="cat-item-dimentions">Габариты: '.$value['diametr'].' x '.$value['length'].'</span>';
      endif;
      if ($value['coating'] != ''):
        $output .= '<span class="cat-item-dimentions">Покрытие: '.$value['coating'].'</span>';
      endif;
      if ($value['strength'] != ''):
        $output .= '<span class="cat-item-dimentions">Класс прочности: '.$value['strength'].'</span>';
      endif;
      if ($value['number'] != ''):
        $output .= '<span class="cat-item-dimentions">Количество: '.$value['number'].' шт. </span>';
      endif;
      if ($value['weight'] != ''):
        $output .= '<span class="cat-item-dimentions">Вес: '.$value['weight'].' кг.</span>';
      endif;

      $output .= '</div>';

      if ($value['nid'] == '') {
        $cost = $n->field_cost['und'][0]['value'];
      } else {
        $cost = $value['sum'];
      }
      if ($_SESSION['cart'][$key]['add_in_cart_nocalc'] == 1) {
        $cost = $_SESSION['cart'][$key]['hidden_sum'];
      }

      $output .= '<div class="cart-col-3 cart-col"><div class="cat-item-prices"><span class="cat-item-price">'.metiz_store_price_formatter($cost).'</span></div></div>';
      $output .= '<div class="cart-col-4 cart-col"><button type="submit" data-key="'.$key.'" data-cost="'.$cost.'" class="cart-del-btn"><i class="fa fa-trash-o" aria-hidden="true"></i></button></div></div>';
    }
    $output .= '<div class="cart-sum blac-btn"><span>Итого: <span id="cart-total-cost">'.metiz_store_price_formatter($_SESSION['cart_cost']).'</span></span></div>';

    $output .= drupal_render(drupal_get_form('metiz_store_checkout_order_form'));
  } else {
    $output = 'Товаров в корзине нет.';
  }

  return $output;
}

function metiz_store_order_cart_success() {
  if (isset($_SESSION['order_id']) && $_SESSION['order_id'] > 0) {
    $oid = $_SESSION['order_id'];
    $phone = $_SESSION['customer_phone'];
    $email = $_SESSION['customer_email'];
    $output = '<h2>Вскоре наши менеджеры с Вами свяжутся</h2><p><strong>Номер Вашего заказа: </strong>№'.$oid.'</p><p><strong>Ваш телефон: </strong>'.$phone.'</p><p><strong>Ваш email: </strong>'.$email.' </br>На него было отправлено письмо с деталями заказа, если не видете его в папке "Входящие" проверьте папку "Спам".</p>';
    unset($_SESSION['customer_name']);
    unset($_SESSION['customer_email']);
    unset($_SESSION['customer_phone']);
    unset($_SESSION['customer_address']);
    unset($_SESSION['cart']);
    unset($_SESSION['cart_cost']);
    unset($_SESSION['order_id']);
    unset($_SESSION['cart_nocalc']);
  } else {
    drupal_goto('<front>');
  }

  return $output;
}
