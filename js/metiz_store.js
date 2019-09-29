/**
 * Metiz Store JS functions.
 */

(function ($) {
  Drupal.behaviors.metiz_storeBehavior = {
    attach: function (context, settings) {
      console.log('metiz_store.js - enabled');

      if ($('body').hasClass('page-admin-store')) {
        $('.order-details').on('click', function() {
          if ($(this).hasClass('show')) {
            $(this).parent().find('table').fadeOut('slow');
            $(this).text('Показать детали заказа').removeClass('show');
          } else {
            $(this).parent().find('table').fadeIn('slow');
            $(this).text('Скрыть детали заказа').addClass('show');
          }
        });

        $('.page-admin-store .orders-table tr i').on('click', function() {
          $('.page-admin-store .orders-table tr td .actions').hide();
          $(this).parent().find('.actions').show();
        });

        $('.page-admin-store .orders-table tr td .actions span').on('click', function() {
          var actionsContainer = $(this).parent();
          var oid = $(this).parent().parent().parent().attr('id');
          var actionType = $(this).attr('class');

          if (actionType != 'cancel') {
            $.ajax({
              type: 'get',
              url: '/metiz_store_order_update',
              data: {oid: oid, action: actionType},
              dataType: 'html',
              success: function (r) {
                if (r == 'success') {
                  actionsContainer.hide();
                }
                if (actionType == 'new') {
                  actionsContainer.parent().parent().attr('class', 'new');
                  actionsContainer.parent().find('.status-text').text('Новый');
                }
                if (actionType == 'in-process') {
                  actionsContainer.parent().parent().attr('class', 'in-process');
                  actionsContainer.parent().find('.status-text').text('В работе');
                }
                if (actionType == 'done') {
                  actionsContainer.parent().parent().attr('class', 'done');
                  actionsContainer.parent().find('.status-text').text('Выполнен');
                }
                if (actionType == 'delete') {
                  actionsContainer.parent().parent().hide();
                  actionsContainer.parent().parent().next().hide();
                }
              }
            });
          } else {
            actionsContainer.hide();
          }
        });
      }

      $('.cart-del-btn').on('click', function() {
        var cid = $(this).data('key');
        var cost = $(this).data('cost');
        var row = $(this).parent().parent();
        $.ajax({
          type: 'post',
          url: '/metiz_store_cart_delete_ajax',
          data: {cid: cid, cost: cost},
          dataType: 'html',
          success: function (r) {
            if (r == 'success') {
              row.hide();
              cart_update();
            }
          }
        });
      });

      $('input.cat-item-add-to-cart').on('click', function() {
        var nid = $(this).parent().find('input[name="nid"]').val();
        var field_cost = $(this).parent().find('input[name="field_cost"]').val();

        $.ajax({
          type: 'post',
          url: '/metiz_store_add_to_cart_ajax',
          data: {action: 'add_in_cart', nid: nid, field_cost: field_cost},
          dataType: 'html',
          success: function (r) {
            if (r == 'success') {
              add_cart_message();
              cart_update();
            }
          }
        });

        return false;
      });

      $('.btn-buy').on('click', function() {
        if (!$(this).hasClass('left-small')) {
          if ($(this).parent().find('input[name="add_in_cart_nocalc"]').length > 0) {
            var flag = 0;
            $(this).parent().find('select').each(function() {
              if ($(this).val() != 'no') {
                flag = 1;
              } else {
                flag = 0;
              }
            });
  
            if (flag == 1) {
              var dataMass = {};
              dataMass.add_in_cart_nocalc = 1;
              $(this).parent().find('input').each(function() {
                var elementName = $(this).attr('name');
                if (!!elementName) {
                  var elementVal = $(this).val();
                  dataMass[elementName] = elementVal;
                }
              });
              $.ajax({
                type: 'post',
                url: '/metiz_store_add_to_cart_ajax',
                data: dataMass,
                dataType: 'html',
                success: function (r) {
                  console.log('debug: ' + r);
                  if (r == 'success') {
                    add_cart_message();
                    cart_update();
                  }
                }
              });
            } else {
              alert ('Сначала выберите все параметры!');
            }
          } else {
            var nid  = $(this).parent().find('input[name="nid"]').val();
            var diametr = $(this).parent().find('select[name="diametr"]').val();
            var length = $(this).parent().find('select[name="length"]').val();
            var coating = $(this).parent().find('select[name="coating"]').val();
            var strength = $(this).parent().find('select[name="strength"]').val();
            var number = $(this).parent().find('input[name="number"]').val();
            var weight = $(this).parent().find('input[name="weight"]').val();
            var sum = $(this).parent().find('input[name="hidden_sum"]').val();
            var single_cost = $(this).parent().find('input[name="single_cost"]').val();
            var single_weight = $(this).parent().find('input[name="single_weight"]').val();
  
            $.ajax({
              type: 'post',
              url: '/metiz_store_add_to_cart_ajax',
              data: {
                nid: nid, 
                diametr: diametr,
                length: length,
                coating: coating,
                strength: strength,
                number: number,
                weight: weight,
                hidden_sum: sum,
                single_cost: single_cost,
                single_weight: single_weight,
              },
              dataType: 'html',
              success: function (r) {
                if (r == 'success') {
                  add_cart_message();
                  cart_update();
                }
              }
            });
          }
          
          return false;
        }
      });

      $('.btn-buy-2').on('click', function() {
        var nid = $(this).parent().find('input[name="nid"]').val();
        var field_cost = $(this).parent().find('input[name="field_cost"]').val();
        var single_cost = $(this).parent().find('input[name="single_cost"]').val();
        var number = $(this).parent().find('input[name="number"]').val();
        $.ajax({
          type: 'post',
          url: '/metiz_store_add_to_cart_ajax',
          data: {
            action: 'single',
            nid:  nid,
            field_cost: field_cost,
            single_cost: single_cost,
            number: number,
          },
          dataType: 'html',
          success: function (r) {
            // console.log(r);
            if (r == 'success') {
              add_cart_message();
              cart_update();
            }
          }
        });

        return false;
      });

      if ($('body').hasClass('page-user')) {
        $('.order-details').on('click', function() {
          if ($(this).hasClass('show')) {
            $(this).parent().find('table').fadeOut('slow');
            $(this).text('Показать детали заказа').removeClass('show');
          } else {
            $(this).parent().find('table').fadeIn('slow');
            $(this).text('Скрыть детали заказа').addClass('show');
          }
        });
      }

      setTimeout(function() {
        cart_update();
      }, 100);

      function cart_update() {
        console.log('cart_update');
        $.ajax({
          type: 'get',
          url: '/metiz_store_cart_block_ajax',
          data: {},
          dataType: 'html',
          success: function (r) {
            console.log(r);
            var data  = r;
            $('#cart_box .quantity').html(r);
            if (r == 'Корзина пуста') {
              $('#cart_box .cart_link').hide();
              $('#metiz-store-checkout-order-form').hide();
              if (!$('body').hasClass('page-shop-cart-success')) {
                $('.page-shop-cart #block-system-main .content').html('Товаров в корзине нет.');
              }
            } else {
              $('#cart_box .cart_link').show();
              $('#metiz-store-checkout-order-form').show();
            }
            if ($('#cart-total-cost').length > 0) {
              $('#cart-total-cost').html($('#cart_box .quantity span').html());
            }
          }
        });
      }

      function add_cart_message() {
        var delay = 1000;
        var message = '<div id="add-cart-message">Товар добавлен в Корзину!</div>';
        $('body').append(message);
        setTimeout(function() {
          $('#add-cart-message').fadeOut( "slow", function() {
            $(this).remove();
          });
        }, delay);
      }
    }
  };
})(jQuery);
