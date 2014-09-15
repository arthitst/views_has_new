<?php
/**
 * @file
 * Easy Social view handler.
 */

/**
 * Handler for displaying Easy Social share buttons in views.
 */
class views_has_new_handler_field extends views_handler_field {
  function init(&$view, &$options) {
      parent::init($view, $options);
      // Declare additional fields to be loaded with a single query.
      // This way there's no need to node_load() on render().
      $this->additional_fields['nid'] = 'nid';
      $this->additional_fields['type'] = 'type';
      $this->additional_fields['language'] = 'language';
      $this->additional_fields['title'] = 'title';
      $this->additional_fields['created'] = 'created';
      $this->additional_fields['changed'] = 'changed';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['custom_date_limit_new'] = array('default' => '');
    $options['custom_date_limit_update'] = array('default' => '');

    return $options;
  }

   function options_form(&$form, &$form_state) {

    $form['custom_date_limit_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Days (new)'),
      '#description' => t('Show each day.'),
      '#default_value' => isset($this->options['custom_date_limit_new']) ? $this->options['custom_date_limit_new'] : '',
    );
    $form['custom_date_limit_update'] = array(
      '#type' => 'textfield',
      '#title' => t('Days (update)'),
      '#description' => t('Show each day.'),
      '#default_value' => isset($this->options['custom_date_limit_update']) ? $this->options['custom_date_limit_update'] : '',
    );


     parent::options_form($form, $form_state);
   }

   function render($values) {
    $custom_format_new = $this->options['custom_date_limit_new'];
    $custom_format_update = $this->options['custom_date_limit_update'];
     // Render your content.
    $node = new stdClass;
    $node->nid = $values->{$this->aliases['nid']};
    $node->type = $values->{$this->aliases['type']};
    $node->language = $values->{$this->aliases['language']};
    $node->title = $values->{$this->aliases['title']};
    $node->created = $values->{$this->aliases['created']};
    $node->changed = $values->{$this->aliases['changed']};

     $now = time();

    if($node->created == $node->changed){
      $node_now = $node->created;

      $diff = abs($node_now - $now);

      $days = floor($diff / (60*60*24));
   
       if($days <= $custom_format_new){
          return '<span class="marker new">new</span>';
       }
    }elseif($node->created != $node->changed){
      $node_now = $node->changed;

      $diff = abs($node_now - $now);

      $days = floor($diff / (60*60*24));
    if ($custom_format_update != 0) {
       if($days <= $custom_format_update){
          return '<span class="marker update">update</span>';
       }
     }
    }
  }
}