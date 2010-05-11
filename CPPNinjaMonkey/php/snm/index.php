<?php 
require_once('debug.inc');
require_once('gamedb.inc');

$db = new GameDB();
$db->initPlayerSession();
$db->commit();

$enc = 'Windows-1252'; //'utf-8';
echo '<' . '?xml version="1.0" encoding="' . $enc . '"?' . '>';
?>
<html>
<head>
    <?php 
    Util::printStylesheets('css/main.css');
    Util::printScripts('scriptaculous-1.8.2/prototype.js', 
        'scriptaculous-1.8.2/scriptaculous.js?load=effects,dragdrop',
        'javascript/rpc.js',
        'javascript/util.js',
        'javascript/main.js',
        'javascript/debug.js',
        'javascript/observerlist.js',
        'javascript/animation.js');
    ?>
</head>
<body onload="init()">
    <script type="text/javascript" language="javascript">
      // <![CDATA[
      var initialState = {
          pieces: <?php echo json_encode($db->getPieces())?>,
          gameID: <?php echo json_encode($db->getGameID())?>,
          playerID: <?php echo json_encode($db->getPlayerID())?>,
          magic: <?php echo json_encode($db->getMagic())?>
      };
      // ]]>
    </script>
    <div class="header">
        Player ID: <?php echo $db->getPlayerID()?> 
        Game ID: <?php echo $db->getGameID()?> 
        Magic: <?php echo $db->getMagic()?>
    </div>
    <div id="pollStatus" colspan="2"></div>
    <div class="pageCenter"><table class="pageCenter"><tr><td>
        <div><table class="piecetray">
            <tr>
                <td>Your pieces:</td>
                <td id="tray"></td>
            </tr>
            <tr><td id="timeBar" colspan="2"></td></tr>
            <tr><td id="board" colspan="2"></td></tr>
        </table></div>
        <div id="commit" onclick="commit_Click(this); return false">[commit]</div>
    </div></td></tr>
</body>
</html>