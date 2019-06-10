<?php
/**
 * Des 对称加密算法类
 * Auther: Dejan
 * Date: 2019-06-09
 */
class Des
{
    /**
     * Des 对称加密 - 加密方法  加密模式：ecb
     * @param  $input  明文字符串
     * @param  $key    des密钥
     * @return String
     */
    static function Encrypt($input, $key)
    {
        $size = mcrypt_get_block_size('des', 'ecb');
        $input = self::pkcs5_pad($input, $size);
        $td = mcrypt_module_open('des', '', 'ecb', '');

        $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);

        @mcrypt_generic_init($td, $key, $iv);
        $data = mcrypt_generic($td, $input);
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);
        $data = base64_encode($data);
        return $data;
    }

    /**
     * Des 对称加密 - 解密方法  加密模式：ecb
     * @param  $input  密文字符串
     * @param  $key    des密钥
     * @return String
     */
    static function Decrypt($input, $key)
    {
        $input = base64_decode($input);
        $size = mcrypt_get_block_size('des', 'ecb');
        $td = mcrypt_module_open('des', '', 'ecb', '');
        $iv = @mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
        @mcrypt_generic_init($td, $key, $iv);
        $data = mdecrypt_generic($td, $input);
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);
        $data = self::pkcs5_unpad($data, $size);
        return $data;
    }

    /**
     * 明文字符串对齐处理
     * PKCS5Padding的blocksize为8字节, PKCS7Padding的blocksize可以为1到255字节.
     */
    private static function pkcs5_pad($text, $blocksize)
    {
        $pad = $blocksize - (strlen($text) % $blocksize);
        return $text . str_repeat(chr($pad), $pad);
    }

    /**
     * 去除对齐处理填补的字符串
     * PKCS5Padding的blocksize为8字节, PKCS7Padding的blocksize可以为1到255字节.
     */
    private static function pkcs5_unpad($text)
    {
        $pad = ord($text{
        strlen($text) - 1});
        if ($pad > strlen($text)) {
            return false;
        }
        if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) {
            return false;
        }
        return substr($text, 0, -1 * $pad);
    }
}
