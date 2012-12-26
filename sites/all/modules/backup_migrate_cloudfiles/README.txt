
-------------------------------------------------------------------------------
Backup and Migrate Rackspace Cloud Files for Drupal 6.x
  by arpieb
-------------------------------------------------------------------------------

DESCRIPTION:
This module provides a destination for the Backup and Migrate module to utilize 
the Rackspace Cloud Files service.

-------------------------------------------------------------------------------

INSTALLATION:
* Put the module in your drupal modules directory and enable it in 
  admin/build/modules.
  
* Due to the fact that Rackspace has released their PHP library wrapper for 
  the Cloud Files API under the MIT license, you will have to install their 
  code separately in order for this module to function.  The code currently 
  lives here:
  
  https://github.com/rackspace/php-cloudfiles/tree
  
  Install it into sites/all/libraries/rackspace/php-cloudfiles so that the 
  file cloudfiles.php is located at:
   
  sites/all/libraries/rackspace/php-cloudfiles/cloudfiles.php
  
* Collect your Cloud Files username and API key (from the Cloud Files control 
  panel).
  
* Add a new Cloudfiles destination via the Backup & Migrate module admin, using 
  the username and API key from above.  

OPTIONAL:
* Enable token.module to allow token replacement in Cloud Files metadata

-------------------------------------------------------------------------------

CLOUD FILES METADATA:
The Cloud Files system is able to attach an arbitrary number of name-value 
pairs to each object in its filesystem.  Seeing as this could be a great 
benefit for annotating backups, if you have the Token module installed and 
enabled, this module gives you the ability to define a name-value pair list to 
attach to your backup file using system-global tokens.

An example set of tokenized name-value pairs would be:

  site-name|[site-name]
  site-mail|[site-mail]
  backup-user-name|[user-name]
  backup-user-id|[user-id]
  backup-user-email|[user-mail]
  backup-date|[site-date-small]

Which results in the following metadata being stored with the backup file in Cloud Files:

  Name                Value
  Site-Name           D6 Dev Local
  Backup-User-Id      1
  Backup-User-Name    admin
  Backup-Date         07/20/2011 - 18:57
  Backup-User-Email   foo@example.com
  Site-Mail           foo@example.com

If any offending characters are found in the name (alphanumeric, dash, and 
underscore are allowed), they are converted to dashes. AFAICT, the value field 
is wide open as far as characters are concerned. The module does call 
check_plain on the value before sending it to Cloud Files to prevent any XSS 
injections. 

-------------------------------------------------------------------------------

RACKSPACE SERVICENET
Rackspace has a high-bandwidth, low-latency, internal network called ServiceNet 
that is available to all Rackspace-hosted servers.  If your site is hosted on a 
Rackspace server, this module will attempt to automagically detect this using 
the host IP address' WHOIS records.  If the server is found to be a Rackspace 
host, the ServiceNet flags are enabled for the Cloud Files connections by this 
module, resulting in very fast - and (at least at the time I am writing this) 
FREE - bandwidth for your backups and restorations.  

Who loves ya?

-------------------------------------------------------------------------------
