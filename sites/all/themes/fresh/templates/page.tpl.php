<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
<?php if (theme_get_setting('social_links', 'fresh')): ?>
<div id="topbar">
<div class="feedlist">
  <ul>
    <li><a href="<?php print $front_page; ?>rss.xml">RSS</a></li>
    <li><a href="http://www.facebook.com/<?php echo theme_get_setting('facebook_username', 'fresh'); ?>" target="_blank" rel="me">Facebook</a></li> 
    <li><a href="http://www.twitter.com/<?php echo theme_get_setting('twitter_username', 'fresh'); ?>" target="_blank" rel="me">Twitter</a></li>
  </ul>
</div>
</div>
<?php endif; ?>
<div id="masthead">
  <div id="top">
    <div class="head">      
    <?php if ($logo): ?>
       <div id="logo">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" /></a>
        </div>
      <?php endif; ?>
      <h1 class="sitename"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
  <div class="clear"></div>	
    </div>
  </div>
	
  <div id="botmenu">
    <a title="Home" href="<?php print $front_page; ?>" class="homemenu"> </a>
    <div id="submenu">
    <?php 
    $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
    print drupal_render($main_menu_tree);
    ?>
    </div>
  </div>
</div>

<div id="wrapper">
 <div id="casing">
	
  <?php if ($is_front): ?>
  <?php if (theme_get_setting('slideshow_display', 'fresh')): ?>
  <?php  $url1 = theme_get_setting('slide1_url', 'fresh');
  $url2 = theme_get_setting('slide2_url', 'fresh');
  $url3 = theme_get_setting('slide3_url', 'fresh'); ?>
    <div id="nivo-cover">
      <div id="nivo-box">
      <div id="slider" class="nivoSlider">
      <a href="<?php print url($url1); ?>"><img class="slideimg" src="<?php print base_path() . drupal_get_path('theme', 'fresh') . '/images/slide-image-1.jpg'; ?>"/></a>
      <a href="<?php print url($url2); ?>"><img class="slideimg" src="<?php print base_path() . drupal_get_path('theme', 'fresh') . '/images/slide-image-2.jpg'; ?>"/></a>
      <a href="<?php print url($url3); ?>"><img class="slideimg" src="<?php print base_path() . drupal_get_path('theme', 'fresh') . '/images/slide-image-3.jpg'; ?>"/></a>
      </div>
      </div><!-- nivo-box -->
    </div> <!-- nivo-cover -->	

  <?php endif; ?>
  <?php endif; ?>

  <?php print render($page['header']); ?>

  <div id="content">
  <?php if (theme_get_setting('breadcrumbs', 'fresh')): ?><div class="breadcrumb"><?php if ($breadcrumb): print $breadcrumb; endif;?></div><?php endif; ?>
  <section id="main" role="main" class="post">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><div class="title"><h2 class="title" id="page-title"><?php print $title; ?></h2></div><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
  </section> <!-- /#main -->
  </div>
  <?php if ($page['sidebar_first']): ?>
    <aside id="sidebar-first" role="complementary" class="sidebar clearfix">
      <?php print render($page['sidebar_first']); ?>
    </aside>  <!-- /#sidebar-first -->
  <?php endif; ?>

<div class="clear"></div>
<?php print render($page['footer']) ?>
</div> 
</div>

<div class="bottomcover">
<div id="bottom">
 <?php if ($page['footer_first']): ?><!-- / start first footer block -->
    <div class="botwid">
      <?php print render($page['footer_first']); ?>
    </div> <!-- / end first footer -->
  <?php endif; ?>
 <?php if ($page['footer_second']): ?><!-- / start second footer block -->
    <div class="botwid">
      <?php print render($page['footer_second']); ?>
    </div> <!-- / end second footer -->
  <?php endif; ?>
 <?php if ($page['footer_third']): ?><!-- / start third footer block -->
    <div class="botwid">
      <?php print render($page['footer_third']); ?>
    </div> <!-- / end third footer -->
  <?php endif; ?>
 <?php if ($page['footer_fourth']): ?><!-- / start fourth footer block -->
    <div class="botwid">
      <?php print render($page['footer_fourth']); ?>
    </div> <!-- / end fourth footer -->
  <?php endif; ?>
<div class="clear"></div>
</div> 
</div> 

<div id="footer">
  <div class="fcred">
    <?php if (theme_get_setting('footer_copyright', 'fresh')): ?>
    <div id="copyright"><?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?>.</div>
    <?php endif; ?>
    <?php if (theme_get_setting('footer_credits', 'fresh')): ?>
    <div id="credits"><?php print t('Thanks to'); ?> <a href="http://www.sidepon.com" target="_blank">Sidepon.com</a> | <?php print t('Developed by'); ?> <a href="http://www.devsaran.com" target="_blank">Devsaran</a>.</div>
    <?php endif; ?>
  </div>
</div>