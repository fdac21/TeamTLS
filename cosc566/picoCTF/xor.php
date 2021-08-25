<?php
$cookie = "ClVLIh4ASCsCBE8lAxMacFMZV2hdVVotEhhUJQNVAmhSEV4sFxFeaAw=";

function xor_enc($data) {
    // $key = json_encode(array("showpassword"=>"no", "bgcolor"=>"#ffffff"));
    $key = "qw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8J";
    $out = '';

    for ($i=0;$i<strlen($data); $i++) {
        $out .= $data[$i] ^ $key[$i % strlen($key)];
    }

    return $out;
}

printf("%s\n", base64_encode(xor_enc(json_encode(array("showpassword"=>"yes", "bgcolor"=>"#ffffff")))));
// printf("%s\n", xor_enc(base64_decode($cookie)));

?>