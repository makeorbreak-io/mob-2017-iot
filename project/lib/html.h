//------------------------------------------------------------------------------
// request
//------------------------------------------------------------------------------
String query(String request, String variable) {
  int begin = request.indexOf(variable + "=");

  if (begin < 0) return "";

  int end = request.indexOf("&", begin);
  if (end < 0) end = request.indexOf(" ", begin);

  return request.substring(begin + 2, end);
}

//------------------------------------------------------------------------------
// HTML
//------------------------------------------------------------------------------
String beginHTML() {
  String html = "\
HTTP/1.1 200 OK\n\
Content-Type: text/html\n\
\n\
<!DOCTYPE HTML>\n\
<html>\n\
  <body>\n\
";

  return html;
}

String endHTML() {
  String html = "\n\
  </body>\n\
</html>\n\
";

  return html;
}

//
// Form inputs

String submit(String label) {
  return "<button type=\"submit\">" + label + "</button>";
}

String input(String type, String name, String label, String value) {
  String html = "\n\
<div>\n\
  <label>\n\
    <span>" + label + "</span>\n\
    <input type=\"" + type + "\" name=\"" + name + "\" value=\"" + value + "\" />\n\
  </label>\n\
</div>";

  return html;
}

String range(String name, String label, int value = 0) {
  String html = "\n\
<div>\n\
  <label>\n\
    <span>" + label + "</span>\n\
    <input type=\"range\" min=\"0\" max=\"255\" name=\"" + name + "\" value=\"" + value + "\" />\n\
  </label>\n\
</div>";

  return html;
}

//
// HTML elements
String div(String content)  { return "<div>"  + content + "</div>"; }
String form(String content) { return "<form>" + content + "</form>"; }
String p(String content)    { return "<p>"    + content + "</p>"; }
