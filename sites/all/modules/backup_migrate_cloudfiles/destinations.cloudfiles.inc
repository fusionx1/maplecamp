<?php

/**
 * @file
 * Functions to handle the Rackspace Cloudfiles backup destination.
 */

/**
 * A destination for sending database backups to a Rackspace Cloudfiles server.
 *
 * @ingroup backup_migrate_destinations
 */
class backup_migrate_destination_cloudfiles extends backup_migrate_destination_remote {
  var $supported_ops = array('scheduled backup', 'manual backup', 'restore', 'list files', 'configure', 'delete');
  var $cloudfiles = NULL; 
  var $cache_files = TRUE;
  var $is_rs_server = FALSE;

  /**
   * Save to to the Cloudfiles destination.
   */
  function save_file($file, $settings) {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    _backup_migrate_cloudfiles_dbg(array('settings' => $settings));
    
    if ($cloudfiles = $this->cloudfiles_object()) {
      try {
        // Open container object
        $cloudfiles->authenticate();
        
        $conn = new CF_Connection($cloudfiles, $this->is_rs_server);
        
        $container = $conn->create_container($this->get_container());
        $file_obj = $container->create_object($file->filename());
        
        // Try saving the file to the cloud:
        try {
          $file_obj->content_type = 'application/x-gzip';
          $file_obj->metadata = $this->get_metadata($settings);
          $file_obj->load_from_filename($file->filepath());
          return $file;
        }
        catch (SyntaxException $e) {
          drupal_set_message(t('File upload - missing required parameters'), 'error', FALSE);
        }
        catch (BadContentTypeException $e) {
          drupal_set_message(t('File upload - no Content-Type was/could be set'), 'error', FALSE);
        }
        catch (MisMatchedChecksumException $e) {
          drupal_set_message(t('File upload - MD5 checksums unequal'), 'error', FALSE);
        }
        catch (IOException  $e) {
          drupal_set_message(t('File upload - error opening file'), 'error', FALSE);
        }
        catch (InvalidResponseException $e) {
          drupal_set_message(t('File upload - unexpected response from Cloud'), 'error', FALSE);
        }
      }
      catch (AuthenticationException $e) {
        drupal_set_message(t('Authentication failed - check username and API key'), 'error', FALSE);
      }
      catch (SyntaxException $e) {
        drupal_set_message(t('Invalid container name given'), 'error', FALSE);
      }
      catch (InvalidResponseException $e) {
        drupal_set_message(t('Unexpected response from Cloud'), 'error', FALSE);
      }
      catch (Exception $e) {
        drupal_set_message(t('Caught default exception: @msg', array('@msg' => $e->getMessage())), 'error', FALSE);
      }
    }
    
    return FALSE;
  }

  /**
   * Load from the Cloudfiles destination.
   */
  function load_file($file_id) {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    
    backup_migrate_include('files');
    $file = new backup_file(array('filename' => $file_id));
    if ($cloudfiles = $this->cloudfiles_object()) {
      try {
        // Open container object
        $cloudfiles->authenticate();
        
        $conn = new CF_Connection($cloudfiles, $this->is_rs_server);

        $container = $conn->create_container($this->get_container());
        $file_obj = $container->create_object($file->filename());
        
        // Try loading the file from the cloud:
        try {
          $file_obj->save_to_filename($file->filepath());
          return $file;
        }
        catch (IOException  $e) {
          drupal_set_message(t('File download - error opening file'), 'error', FALSE);
        }
        catch (InvalidResponseException $e) {
          drupal_set_message(t('File download - unexpected response from Cloud'), 'error', FALSE);
        }
      }
      catch (AuthenticationException $e) {
        drupal_set_message(t('Authentication failed - check username and API key'), 'error', FALSE);
      }
      catch (SyntaxException $e) {
        drupal_set_message(t('Invalid container name given'), 'error', FALSE);
      }
      catch (InvalidResponseException $e) {
        drupal_set_message(t('Unexpected response from Cloud'), 'error', FALSE);
      }
      catch (Exception $e) {
        drupal_set_message(t('Caught default exception: @msg', array('@msg' => $e->getMessage())), 'error', FALSE);
      }
    }
    
    return NULL;
  }

  /**
   * Delete from the Cloudfiles destination.
   */
  function delete_file($file_id) {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    
    if ($cloudfiles = $this->cloudfiles_object()) {
      try {
        // Open container object
        $cloudfiles->authenticate();
        
        $conn = new CF_Connection($cloudfiles, $this->is_rs_server);
        
        $container = $conn->create_container($this->get_container());
        
        // Try deleting the file from the cloud:
        try {
          $container->delete_object($file_id);
        }
        catch (SyntaxException $e) {
          drupal_set_message(t('File delete - invalid Object name'), 'error', FALSE);
        }
        catch (NoSuchObjectException  $e) {
          drupal_set_message(t('File download - remote Object does not exist'), 'error', FALSE);
        }
        catch (InvalidResponseException $e) {
          drupal_set_message(t('File download - unexpected response from Cloud'), 'error', FALSE);
        }
      }
      catch (AuthenticationException $e) {
        drupal_set_message(t('Authentication failed - check username and API key'), 'error', FALSE);
      }
      catch (SyntaxException $e) {
        drupal_set_message(t('Invalid container name given'), 'error', FALSE);
      }
      catch (InvalidResponseException $e) {
        drupal_set_message(t('Unexpected response from Cloud'), 'error', FALSE);
      }
      catch (Exception $e) {
        drupal_set_message(t('Caught default exception: @msg', array('@msg' => $e->getMessage())), 'error', FALSE);
      }
    }
  }

  /**
   * List all files from the Cloudfiles container.
   */
  function list_files() {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    
    backup_migrate_include('files');
    $files = array();
    if ($cloudfiles = $this->cloudfiles_object()) {
      try {
        // Open container object
        _backup_migrate_cloudfiles_dbg('authenticating');
        $cloudfiles->authenticate();
        
        _backup_migrate_cloudfiles_dbg('connecting');
        $conn = new CF_Connection($cloudfiles, $this->is_rs_server);
         
        _backup_migrate_cloudfiles_dbg('create/retrieve container: ' . $this->get_container());
        $container = $conn->create_container($this->get_container());
        
        // Try loading the file list from the cloud:
        try {
          _backup_migrate_cloudfiles_dbg('getting object list');
          $obj_list = $container->get_objects();
          foreach ($obj_list as $obj) {
            $info = array(
              'filename' => $obj->name,
              'filesize' => $obj->content_length,
              'filetime' => strtotime($obj->last_modified . '-0000'),
            );
            $files[$info['filename']] = new backup_file($info);
          }
        }
        catch (InvalidResponseException $e) {
          drupal_set_message(t('File list - unexpected response from Cloud'), 'error', FALSE);
        }
      }
      catch (AuthenticationException $e) {
        drupal_set_message(t('Authentication failed - check username and API key'), 'error', FALSE);
      }
      catch (SyntaxException $e) {
        drupal_set_message(t('Invalid container name given'), 'error', FALSE);
      }
      catch (InvalidResponseException $e) {
        drupal_set_message(t('Unexpected response from Cloud'), 'error', FALSE);
      }
      catch (Exception $e) {
        drupal_set_message(t('Caught default exception: @msg', array('@msg' => $e->getMessage())), 'error', FALSE);
      }
    }
    
    return $files;
  }

  /**
   * Get the form for the settings for this filter.
   */
  function edit_form() {
    // Check for the library.
    $this->cloudfiles_object();

    $form = parent::edit_form();
    $form['scheme']['#type'] = 'value';
    $form['scheme']['#value'] = 'rscf';
    $form['host']['#type'] = 'value';
    $form['host']['#value'] = 'cloudfiles.rackspacecloud.com';

    $form['path']['#title'] = 'Cloudfiles Container';
    $form['path']['#default_value'] = $this->get_container();
    $form['path']['#description'] = 'If this container does not already exist, it will be created for you.';

    $form['user']['#title'] = 'Username';
    $form['pass']['#title'] = 'API Key';

    $form['settings']['#weight'] = 50;
    $form['settings']['#tree'] = TRUE;

    // Give the user the option to use system-level tokens for CF meta tags
    $metadata = $this->settings('metadata');
    $form['settings']['metadata'] = array(
      '#type' => 'textarea',
      '#title' => t('Cloud File Metadata'),
      '#description' => t('The metadata entries to tag each backup file with in the Cloud Files system. Enter one value per line, in the format <strong>name|value</strong>. The <strong>name</strong> cannot contain spaces, only alhpanumeric values, dashes, and undescores.  The <strong>value</strong> can contain any text as long as it resides on one line; it may also contain any system tokens.'),
      '#rows' => 10,
      '#cols' => 60,
      '#default_value' => (!empty($metadata)) ? $metadata : "site-name|[site:name]\nsite-mail|[site:mail]",
    );

    // If the Token module is installed and enabled, offer help for tokens available for metadata tags
    if (module_exists('token')) {
      $form['settings']['token_help'] = array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['settings']['token_help']['global'] = array(
        '#theme' => 'token_tree',
        '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
        '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.      );
      );
    }
    
    _backup_migrate_cloudfiles_dbg($form);
    return $form;
  }

  /**
   * Submit the form for the settings for the Cloudfiles destination.
   */
  function edit_form_submit($form, &$form_state) {
    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Get the container which is the first part of the path.
   */
  function get_container() {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    if (!empty($this->dest_url)) {
      _backup_migrate_cloudfiles_dbg($this->dest_url);
    }
        
    $parts = explode('/', @$this->dest_url['path']);
    return $parts[0];
  }
  
  function cloudfiles_object() {
    _backup_migrate_cloudfiles_dbg('Entering ' . __FUNCTION__);
    if (!empty($this->dest_url)) {
      _backup_migrate_cloudfiles_dbg($this->dest_url);
    }

    if (file_exists('sites/all/libraries/rackspace/php-cloudfiles/cloudfiles.php')) {
      require_once('sites/all/libraries/rackspace/php-cloudfiles/cloudfiles.php');
      if (!$this->cloudfiles && !empty($this->dest_url['user'])) {
        _backup_migrate_cloudfiles_dbg('creating CF_Authentication');
        
        $this->cloudfiles = new CF_Authentication($this->dest_url['user'], $this->dest_url['pass']);
        $this->cloudfiles->ssl_use_cabundle();

        _backup_migrate_cloudfiles_dbg('created CF_Authentication');

        // Check to see if we're on a Rackspace host
        $this->is_rs_server = _backup_migrate_cloudfiles_is_rs_server();
        if ($this->is_rs_server) {
          _backup_migrate_cloudfiles_dbg('using Rackspace ServiceNet connections');
        }
        else {
          _backup_migrate_cloudfiles_dbg('not using Rackspace ServiceNet connections');
        }
      }
      return $this->cloudfiles;
    }
    else {
      drupal_set_message(t('Due to drupal.org code hosting policies, the PHP binding for Rackspace Cloudfiles cannot be distributed with this module. You must download the library from !link and place it in the directory sites/all/libraries/rackspace/php-cloudfiles.', array('!link' => l('https://github.com/rackspace/php-cloudfiles', 'https://github.com/rackspace/php-cloudfiles'))), 'error', FALSE);
      return NULL;
    }
  }
  
  // Util function to build the metadata array for the Cloud File object from our tokenized values
  function get_metadata($settings) {
    $metadata = array();
    
    if (!empty($settings->destination->settings['metadata'])) {
      _backup_migrate_cloudfiles_dbg(array('Raw CF metadata' => $settings->destination->settings['metadata']));
      $lines = explode("\n", $settings->destination->settings['metadata']);
      foreach ($lines as $line) {
        // Toss out empty lines
        $line = trim($line);
        if (!empty($line)) {
          $tmp = explode('|', $line, 2);
          if (2 == count($tmp)) {
            // OK, we have a name and value pair; clean up name:
            $name = preg_replace('/[^0-9,a-z,A-Z,\-_]/', '-', trim($tmp[0]));
            
            // Replace tokens in value string
            $value = token_replace(trim($tmp[1]));
            
            // Store off in array for return
            $metadata[$name] = check_plain($value);
          }
        }
      }
    }
    
    _backup_migrate_cloudfiles_dbg(array('Expanded CF metadata' => $metadata));
    return $metadata;
  }
}


