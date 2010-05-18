<?php
class Util
{
    public static function getSanitizedInt($value)
    {
        return $value === null ? null : intval($value);
    }
    
    public static function getParameter($key)
    {
        //$value = array_key_exists($key, $_POST) ? $_POST[$key] : '';
        $value = array_key_exists($key, $_GET) ? $_GET[$key] : '';
        return get_magic_quotes_gpc() ? stripslashes($value) : $value;
    }
    
    public static function getMagic($length)
    {
        $digits = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        $magic = '';
        for ($i = 0; $i < $length; ++$i)
        {
            $magic .= $digits{rand(0, strlen($digits) - 1)};
        }
        return $magic;
    }
    
    /**
     * @desc Render script tags including the given set of scripts.
     */
    public static function printScripts(/*...*/)
    {
        $args = func_get_args();
        foreach (Util::flatten($args) as $script)
        {
            //language="javascript"
            ?><script src="<?=$script?>" type="text/javascript" language="JavaScript"></script>
            <?php
        }
    }
    
    public static function printStylesheets(/*...*/)
    {
        $args = func_get_args();
        foreach (Util::flatten($args) as $stylesheet)
        {
            //language="javascript"
            ?><link rel="stylesheet" href="<?=$stylesheet?>" />
            <?php
        }
    }
        
    public static function blank($width, $height)
    {
        ?><img src="images/blank.png" width="<?= $width?>" height="<?= $height?>" /><?php
    }
    
    /**
     * @desc flattens the given tree into a flat list of scalars.
     */
    public static function flatten($array)
    {
        $out = array();
        foreach ($array as $element)
        {
            if (is_array($element))
            {
                $out = array_merge($out, Util::flatten($element));
            }
            else
            {
                $out[] = $element;
            }
        }
        return $out;
    }
}
?>
