<?php
/**
 * @file
 * Administration settings form area for Social Share Privacy module
 */

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
        '#default_value' => variable_get('socialshareprivacy_services', array(0, 0, 0)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google Plus',
        ),
    );

    $form['socialshareprivacy_services_permaoption'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enabled perma_options'),
        '#default_value' => variable_get('socialshareprivacy_services_permaoption', array(0, 0, 0)),
        '#options' => array(
            'facebook' => 'Facebook',
            'twitter' => 'Twitter',
            'gplus' => 'Google Plus',
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