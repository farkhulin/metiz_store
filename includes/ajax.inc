<?php
/**
 * Metiz Store module AJAX functions.
 */

function metiz_store_order_update() {
  if ($_GET['action'] == 'new') {
    $oidStatus = 0;
  }
  if ($_GET['action'] == 'in-process') {
    $oidStatus = 1;
  }
  if ($_GET['action'] == 'done') {
    $oidStatus = 2;
  }
  if (isset($_GET['oid']) && isset($_GET['action']) && $_GET['action'] != 'delete') {
    db_query("UPDATE orders SET status=".$oidStatus." WHERE oid=".$_GET['oid']);
  }
  if (isset($_GET['oid']) && isset($_GET['action']) && $_GET['action'] == 'delete') {
    db_query("DELETE FROM orders WHERE oid=".$_GET['oid']);
    db_query("DELETE FROM orders_data WHERE oid=".$_GET['oid']);
  }
  print ('success');
}

function metiz_store_cart_delete_ajax() {
  $_SESSION['cart_cost'] = $_SESSION['cart_cost'] - $_POST['cost'];
  unset($_SESSION['cart'][$_POST['cid']]);
  print ('success');
}

function metiz_store_add_to_cart_ajax() {
  if (isset($_POST['action']) && $_POST['action'] == 'add_in_cart') {
    $flag = 1;
    foreach ($_SESSION['cart'] as $key => $value) {
      if ($value['nid'] == $_POST['nid']) {
        if (isset($_POST['number']) && $_POST['number'] > 0) {
          $_SESSION['cart'][$key]['number'] += $_POST['number'];
        } else {
          $_SESSION['cart'][$key]['number'] += 1;
        }
        $_SESSION['cart'][$key]['sum'] += $_POST['field_cost'];
        $_SESSION['cart_cost'] += $_POST['field_cost'];
        $flag = 0;
      }
    }
  
    if ($flag) {
      if (isset($_POST['number']) && $_POST['number'] > 0) {
        $array_cart = array(
          'nid' => (int)$_POST['nid'],
          'number' => $_POST['number'],
          'sum' => $_POST['field_cost'],
        );
      } else {
      $array_cart = array(
        'nid' => (int)$_POST['nid'],
        'number' => 1,
        'sum' => $_POST['field_cost']);
      }
  
      $_SESSION['cart'][] = $array_cart;
      $_SESSION['cart_cost'] += $_POST['field_cost'];
    }
  }

  if (isset($_POST['action']) && $_POST['action'] == 'single') {
    if (isset($_POST['number']) && $_POST['number'] > 0) {
      $array_cart = array(
        'nid' => (int)$_POST['nid'],
        'number' => $_POST['number'],
        'sum' => $_POST['field_cost'],
      );
    } else {
      $array_cart = array(
        'nid' => (int)$_POST['nid'],
        'number' => 1,
        'sum' => $_POST['field_cost'],
      );
    }

    $_SESSION['cart'][] = $array_cart;
    $_SESSION['cart_cost'] += $_POST['field_cost'];
  }

  if ((isset($_POST['hidden_sum'])) && ($_POST['hidden_sum'] != '') && ($_POST['hidden_sum'] != 'NaN')) {
    if (isset($_POST['add_in_cart_nocalc'])) {

    } else {
      $array_cart = array(
        'nid' => $_POST['nid'],
        'diametr' => $_POST['diametr'],
        'length' => $_POST['length'],
        'coating' => $_POST['coating'],
        'strength' => $_POST['strength'],
        'number' => $_POST['number'],
        'weight' => $_POST['weight'],
        'sum' => $_POST['hidden_sum'],
        'single_cost' => $_POST['single_cost'],
        'single_weight' => $_POST['single_weight'],
      );
      $_SESSION['cart'][] = $array_cart;
      $_SESSION['cart_cost'] += $_POST['hidden_sum'];
    }
  }

  if (isset($_POST['add_in_cart_nocalc'])) {
    $_SESSION['cart'][] = $_POST;
    $_SESSION['cart_nocalc'] = 1;
    $_SESSION['cart_cost'] += $_POST['hidden_sum'];
  }
  print ('success');
}

function metiz_store_cart_block_ajax() {
  $cartCounter = count($_SESSION['cart']);
  $cartTotal = $_SESSION['cart_cost'];
  if ($cartCounter > 0) {
    print ($cartCounter.' товаров на <span>'.metiz_store_price_formatter($cartTotal).'</span>');
  } else {
    print ('Корзина пуста');
  }
}