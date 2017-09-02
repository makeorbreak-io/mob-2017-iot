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

String input(String type, String name, String value) {
  return "<input type=\"" + type + "\" name=\"" + name + "\" value=\"" + value + "\" />";
}
