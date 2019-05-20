<?php

/**
 * 导出csv文件 - utf-8编码
 */
function csv_to_utf8_out($src_file, $dst_file)
{
	//ob_clean();
	ob_start(); # 读取缓冲区开始
	header('Content-Type: text/csv; charset=utf-8'); # 申明头信息 utf-8编码格式
	echo file_get_contents($src_file);
	$csv_content = "\xEF\xBB\xBF"; # BOM头
	$csv_content .= ob_get_clean();
	file_put_contents($dst_file, $csv_content);
}


csv_to_utf8_out('./src/gz_hr_need-2.csv','./dst/out.csv');