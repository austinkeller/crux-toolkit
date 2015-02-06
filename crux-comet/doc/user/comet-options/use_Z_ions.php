<?php include "head.php" ; ?>
<html>
<body>
<?php include "topmenu.php" ; ?>
<?php include "imgbar.php" ; ?>

<div id="page">
   <div id="content_full">
      <div class="post hr">

         <h2>Comet parameter: use_Z_ions</h2>

         <ul>
         <li>Controls whether or not Z-dot ions are considered in the search.
         <li>Valid values are 0 and 1.
         <li>To not use Z-dot ions, set the value to 0.
         <li>To use Z-dot ions, set the value to 1.
         </ul>

         <p>Example:
         <br><tt>use_Z_ions = 0</tt>
         <br><tt>use_Z_ions = 1</tt>

      </div>
   </div>
   <div style="clear: both;">&nbsp;</div>
</div>

<?php include "footer.php" ; ?>
</html>
