#include "html.h"

String beginHTML() {
  String html = "\
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
    <input type=\"range\" min=\"0\" max=\"1023\" name=\"" + name + "\" value=\"" + value + "\" />\n\
  </label>\n\
</div>";

  return html;
}
