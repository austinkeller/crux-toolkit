<?php include "head.php" ; ?>
<html>
<body>
<?php include "topmenu.php" ; ?>
<?php include "imgbar.php" ; ?>

<div id="page">
   <div id="content_full">
      <div class="post hr">

         <h2>Comet parameter: add_X_user_amino_acid</h2>

         <ul>
         <li>This parameter allows users to define their own custom residue. Just
             encode the letter 'X' in the input FASTA file and specify its mass here.
         <li>The letter 'X' has no default mass.  So the mass entered here will
             be its residue mass.
         <li>The default value is "0.0" if this parameter is missing.  If any peptide
             contains the letter 'X' while this parameter value is set to 0.0, that
             peptide will not be analyzed.
         </ul>

         <p>Example:
         <br><tt>add_X_user_amino_acid = 100.8</tt>

      </div>
   </div>
   <div style="clear: both;">&nbsp;</div>
</div>

<?php include "footer.php" ; ?>
</html>
