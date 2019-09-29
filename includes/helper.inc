<?php
/**
 * Metiz Store module Utility functions.
 */

function metiz_store_price_formatter($price) {
  $price = number_format($price , 2, '.', ' ' ).' &#x20bd;';

  return $price;
}
