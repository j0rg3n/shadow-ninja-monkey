<?php
require_once('debug.inc');
require_once('util.inc');
require_once('piece.inc');

class GameDB
{
    public function GameDB()
    {
        $this->openConnection();
    }
    
    public function destroy()
    {
        $this->closeConnection();
    }
    
    /**
     * @param array $pieces
     */
    public function handleCommitBoard($pieces)
    {
        $this->updatePieces($pieces);
        $this->commit();
        return $this->getPieces();
    }
    
    public function handlePoll()
    {
        // TODO: Don't open the transaction at all when polling.
        $this->commit();
        
        for ($i = 0; $i < 20; ++$i)
        {
            // TODO: Do poll of values/latest revision in DB. DB must contain log of the game.
            echo ' ';
            sleep(25);
        }
        return 'Polled.';
    }

    public function getGameID()
    {
        return $this->gameID;
    }
    
    public function getPlayerID()
    {
        return $this->playerID;
    }
    
    public function getMagic()
    {
        return $this->magic;
    }
    
    public function getPieces()
    {
        return $this->pieces;
    }
    
    public function commit()
    {
        $this->queryOrDie('COMMIT', 'commit transaction');
    }
    
    public function initPlayerSession()
    {
        session_start();
        if (array_key_exists('playerID', $_SESSION))
        {
            $this->playerID = $_SESSION['playerID'];
        }
        else
        {
            $this->gameID = $this->getRunningGame();
            $_SESSION['playerID'] = $this->playerID = $this->createPlayer();
            $this->addPiece(PieceFactory::GetPiece($this->playerID, 'Player', null, null, PLAYER_STATE_NOT_STARTED));
            for ($i = 0; $i < 4; ++$i)
            {
                $this->addPiece(PieceFactory::GetRandomPiece($this->playerID));
            }
            $this->commit();
        }
        session_write_close();
        
        $this->loadPlayer();
        $this->loadPieces();
    }
    
    public function loadPlayer()
    {
        $result = $this->queryOrDie('SELECT `magic`, `game_id` FROM `player` WHERE `id`=' . $this->playerID, 'running games');
        list($this->magic, $this->gameID) = mysql_fetch_row($result);
        mysql_free_result($result);
    }
    
    public function loadPieces()
    {
        $this->pieces = array();

        $result = $this->queryOrDie('SELECT `id`, `player_id`, `type`, `r`, `c`, `state` FROM `piece` WHERE `game_id`=' . $this->gameID, 'pieces');
        while ($row = mysql_fetch_assoc($result))
        {
            $this->pieces[$row['id']] = PieceFactory::GetPiece($row['player_id'], $row['type'], $row['r'], $row['c'], $row['state']);
        }
        mysql_free_result($result);
    }

    /**
     * Performs diff of the structures, and performs inserts, updates and deletes as necessary.
     * @param array $pieces Associative array retreived using JSON.
     */
    public function updatePieces($pieces)
    {
        foreach ($this->pieces as $id => $piece)
        {
            // Rules: Ignore other player's pieces.
            if ($piece->player == $this->playerID && array_key_exists($id, $pieces))
            {
                $newPiece = $pieces[$id];
                $newState = $newPiece['state'];
                $newR = Util::getSanitizedInt($newPiece['r']);
                $newC = Util::getSanitizedInt($newPiece['c']);
                if ($newState != $piece->state || $newR != $piece->r || $newC != $piece->c)
                {
                    // TODO: Rules: Make sure the move is legal
                    // TODO: Rules: Make sure the state is legal
                    $query = 'UPDATE `piece` ' .
                        'SET `r`=' . $this->getSQLInt($newR) . ', ' . 
                        '`c`=' . $this->getSQLInt($newC)  . ', ' .
                        '`state`=' . $this->getSQLString($newState) . ' ' . 
                        'WHERE `id` = ' . $id;
                    $this->queryOrDie($query, 'update piece');
                    
                    $piece->r = $newR;
                    $piece->c = $newC;
                    $piece->state = $newState;
                }
            }
        }
    }
    
    private function createPlayer()
    {
        $insert = 'INSERT INTO `player` (`game_id`, `magic`) VALUES (' . $this->gameID . ', ' . '\'' . Util::getMagic(16) . '\')';
        $this->queryOrDie($insert, 'create new player');
        return mysql_insert_id($this->connection);
    }
    
    /**
     * @desc Add the given piece to the game (only to the database!).
     * @param PlayerPiece $piece
     */
    private function addPiece($piece)
    {
        $query = 'INSERT INTO `piece` (`game_id`, `player_id`, `type`, `r`, `c`, `state`) VALUES (' .
            $this->getSQLInt($this->gameID) . ', ' . 
            $this->getSQLInt($piece->player) . ', ' .
            $this->getSQLString($piece->type) . ', ' .
            $this->getSQLInt($piece->r) . ', ' .
            $this->getSQLInt($piece->c) . ', ' .
            $this->getSQLString($piece->state) . ')';
        $this->queryOrDie($query, 'add piece');
    }
    
    /**
     * @desc Get existing running game, or create a new one if none exists.
     */
    private function getRunningGame()
    {
        // Find old game
        // TODO: Count games older than, say, 2 hours, as ended.
        $result = $this->queryOrDie('SELECT `id` FROM `game` WHERE `end` IS NULL', 'running games');
        $row = mysql_fetch_row($result);
        mysql_free_result($result);
        if ($row)
        {
            return $row[0];
        }
        
        // Create new game
        $this->queryOrDie('INSERT INTO `game` (`start`) VALUES (NOW())', 'create new game');
        return mysql_insert_id($this->connection);
    }
    
    private function queryOrDie($query, $objective)
    {
        $result = mysql_query($query, $this->connection);
        if ($result == null)
        {
            $message = 'Failed sql query for "' . $objective . '"';
            if (DEBUG)
            {
                $message .= ': "' . htmlspecialchars($query) . '"';
                $message .= ': ' . htmlspecialchars(mysql_error($this->connection));
            }
            die($message);
        }
        return $result;
    }

    private function openConnection()
    {
        register_shutdown_function(array($this, 'destroy'));

        $password = trim(file_get_contents('secret.txt'));
        $this->connection = mysql_connect('localhost', 'fabeljet_com', $password);
        if ($this->connection == null)
        {
            die('Could not connect to mysql server.');
        }

        mysql_select_db('fabeljet_com', $this->connection) || die('Could not select database.');
        
        $this->queryOrDie('START TRANSACTION', 'start transaction');
    }

    private function closeConnection()
    {
        if ($this->connection)
        {
            mysql_close($this->connection);
        }
    }

    private function getSQLInt($value)
    {
        return $value === null ? 'NULL' : strval($value);
    }
    
    private function getSQLString($value)
    {
        return $value === null ? 'NULL' : '\'' . mysql_real_escape_string($value, $this->connection) . '\'';
    }

    private $gameID;
    private $playerID;
    private $magic;
    private $connection;
    private $pieces;
}
?>
