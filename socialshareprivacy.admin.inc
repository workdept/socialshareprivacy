<?php
/**
 * @file
 * Administration settings form area for Social Share Privacy module
 */

require_once(DRUPAL_ROOT . '/sites/all/modules/socialshareprivacy/socialshareprivacy.install');

 /**
 * General configuration form for controlling the behaviour.
 */
function socialshareprivacy_admin_settings() {
    $form = array();

    $socialshareprivacy_path = _socialshareprivacy_get_path();

    $form['socialshareprivacy_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => $socialshareprivacy_path,
      '#description' => t('The location where SocialSharePrivacy plugin is installed. Relative paths are from the Drupal root directory.'),
      '#after_build' => array('_socialshareprivacy_admin_settings_check_plugin_path'),
    );

    $form['socialshareprivacy_services'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enabled services'),
        '#default_value' => variable_get('socialshareprivacy_services', unserialize(SOCIALSHAREPRIVACY_DEFAULT_VALUE_SERVICES)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google+',
        ),
    );

    $form['socialshareprivacy_services_permaoption'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enabled Prema-options'),
        '#default_value' => variable_get('socialshareprivacy_services_permaoption', unserialize(SOCIALSHAREPRIVACY_DEFAULT_VALUE_SERVICES)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google+',
        ),
    );

    $form['socialshareprivacy_facebook_action'] = array(
      '#type' => 'radios',
      '#title' => t('Facebook Button action'),
      '#options' => array(
        'recommend' => t('Recommend'),
        'like' => t('Like'),
      ),
      '#default_value' => variable_get('socialshareprivacy_facebook_action', 'recommend'),
    );

    $form['socialshareprivacy_ext_link'] = array(
      '#type' => 'radios',
      '#title' => t('External link (Info icon)'),
      '#default_value' => variable_get('socialshareprivacy_ext_link', ''),
      '#options' => array(
        '' => ('no special aciton: Link will change the actual window.'),
        'blank' => ('Change Link to a _blank window.'),
        'remove' => ('Remove the click event of the link.'),
      ),
    );

    $form['socialshareprivacy_ext_link_target'] = array(
      '#type' => 'textfield',
      '#title' => t('Change default info link'),
      '#description' => t('Specify here a url if you dont want to link to heise.'),
      '#default_value' => variable_get('socialshareprivacy_ext_link_target', ''),
    );

    $form['socialshareprivacy_js_minified'] = array(
      '#type' => 'radios',
      '#title' => t('Use minified javascript file'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_js_minified', 'true'),
    );

    $form['socialshareprivacy_force_german'] = array(
      '#type' => 'radios',
      '#title' => t('Make it all german; use default texts from the plugin and dismiss translations'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_force_german', 'false'),
    );

    $form['socialshareprivacy_neutral_button'] = array(
      '#type' => 'radios',
      '#title' => t('Use language neutral FB button'),
      '#options' => array(
        'true' => t('Yes'),
        'false' => t('No'),
      ),
      '#default_value' => variable_get('socialshareprivacy_neutral_button', 'false'),
    );
    
    $form['socialshareprivacy_cookie_expires'] = array(
      '#type' => 'textfield',
      '#title' => t('Cookie expires'),
      '#description' => t('Expiration time of the Cookie.'),
      '#default_value' => variable_get('socialshareprivacy_cookie_expires', '365'),
    );

    $form['socialshareprivacy_referrertrack'] = array(
      '#type' => 'textfield',
      '#title' => t('Tracking parameter'),
      '#description' => t('The tracking parameter will be added to the end of the URL which is useful for tracking the referrer.'),
      '#default_value' => variable_get('socialshareprivacy_referrertrack', ''),
    );

    return system_settings_form($form);
}

function socialshareprivacy_admin_fieldmgmt() {

  if (!module_exists('field_ui')) {
    drupal_set_message($t('Enable the Field UI module to adjust the Social Share Privacy field settings, manage node fields or manage the display.'), 'warning');
  }

    $ret[] = array('#markup' => t('Enable/Disable the Socialshareprivacy field for your content types (disabling will delete the data for all nodes of the content type):<br/>'));
    $ret[] = drupal_get_form('socialshareprivacy_form_admin_fieldmgmt');
    return $ret;
}

function socialshareprivacy_form_admin_fieldmgmt($form, &$form_state) {
    $form = array();

    $head = array(
      'contenttype' => array('data' => t('Content types'), 'field' => 'contenttype'),
    );

    $options = array();
    $default_values = array();
    foreach (node_type_get_names() as $name => $value) {
      $options['ssp_fieldmgmt_' . $name] = array(
        'contenttype' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $value,
            '#href' => 'admin/structure/types/manage/' . $name . '/fields',
          ),
        ),
      );
      $is_enabled = 0;
      if (field_info_instance('node', variable_get('socialshareprivacy_field_name'), $name)) {
        $is_enabled = 1;
      }
      $default_values['ssp_fieldmgmt_' . $name] = $is_enabled;
    }

    $form['ssp_fieldmgmt'] = array(
      '#type' => 'tableselect',
      '#header' => $head,
      '#options' => $options,
      '#default_value' => $default_values,
      '#empty' => t('No content available.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    return $form;
}

function socialshareprivacy_form_admin_fieldmgmt_submit($form, &$form_state) {
  $form_values =  $form_state['values'];
  $ssp_fieldmgmt = $form_values['ssp_fieldmgmt'];
  $entity = 'node';
  foreach ($ssp_fieldmgmt as $k => $v) {
    if ($v===0) { // this means checked ..
      __remove_custom_field($entity, drupal_substr($k, 14));
    }
    else {
      __add_custom_field($entity, drupal_substr($k, 14));
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}