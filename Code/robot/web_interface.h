#define HTML_CODE "<html>"\
"<head>"\
"<style>"\
"    .board {"\
"         display:grid;"\
"         grid-template-columns: 1fr 1fr 1fr;"\
"         grid-template-areas:"\
"    \". M1 .\""\
"    \"G2 S D2\""\
"    \". M3 .\";}"\
"    button {font-size:50px;}"\
"    #M1 {grid-area:M1;}"\
"    #G2 {grid-area:G2;}"\
"    #D2 {grid-area:D2;}"\
"    #M3 {grid-area:M3;}"\
"    #S{grid-area:S;}"\
"</style>"\
"</head>"\
"<body>"\
"<div class=\"board\">"\
"<div id=\"M1\"><a href=\"/A\" ><button type=\"button\">A</button></a></div>"\
"<div id=\"G2\"><a href=\"/G\"><button type=\"button\">G</button></a></div>"\
"<div id=\"S\"><a href=\"/S\"><button type=\"button\">S</button></a></div>"\
"<div id=\"D2\"><a href=\"/D\"><button type=\"button\">D</button></a></div>"\
"<div id=\"M3\"><a href=\"/R\"><button type=\"button\">R</button></a></div>"\
"</div>"\
"</body>"\
"</html"\
