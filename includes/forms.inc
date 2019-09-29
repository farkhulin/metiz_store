<?php
/**
 * Metiz Store module Forms.
 */

function metiz_store_checkout_order_form($form, &$form_state) {
  global $user;

  if ($user->uid > 0) {
    $customerName = $user->name;
    $customerMail = $user->mail;
    $customerPhone = db_query("SELECT field_user_phone_value FROM field_data_field_user_phone WHERE entity_id=".$user->uid)->fetchField();
    $customerAddress = db_query("SELECT field_user_address_value FROM field_data_field_user_address WHERE entity_id=".$user->uid)->fetchField();
    $enterOrRegister = '';
  } else {
    $customerName = '';
    $customerMail = '';
    $customerPhone = '';
    $customerAddress = '';
    $enterOrRegister = '<a href="/user/register">Зарегистрируйтесь!</a> или <a href="/user">Войдите в Личный Кабинет</a></br>И тогда Вы сможете видеть статус Ваших заказов в Личном кабинете!';
  }

  $form['header'] = array(
    '#markup' => t('<h3>Оформить заказ</h3>'.$enterOrRegister),
  );

  $form['name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Имя'),
    // '#size' => 10,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#default_value' => $customerName,
    '#attributes' => array(
      'placeholder' => t('Введите имя'),
    ),
  );

  $form['phone'] = array(
    '#type' => 'textfield', 
    '#title' => t('Телефон'),
    '#maxlength' => 100,
    '#required' => TRUE,
    '#default_value' => $customerPhone,
    '#attributes' => array(
      'placeholder' => t('Введите телефон'),
    ),
  );

  $form['email'] = array(
    '#type' => 'textfield', 
    '#title' => t('Email'),
    '#maxlength' => 100,
    '#default_value' => $customerMail,
    '#attributes' => array(
      'placeholder' => t('Введите e-mail'),
    ),
  );

  $form['address'] = array(
    '#type' => 'textfield', 
    '#title' => t('Адрес'),
    '#maxlength' => 255,
    '#default_value' => $customerAddress,
    '#attributes' => array(
      'placeholder' => t('Введите адрес'),
    ),
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Заказать'),
  );
  
  return $form;
}

function metiz_store_checkout_order_form_validate($form, &$form_state) {
  // $form_state['rebuild'] = TRUE;
  // if (!($form_state['values']['name'] > 0)){
    // form_set_error('name', t('Price must be a positive number.'));
  // }
}

function metiz_store_checkout_order_form_submit($form, &$form_state) {
  // code...
  metiz_store_add_order();
}
