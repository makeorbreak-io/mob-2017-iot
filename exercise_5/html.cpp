#include "html.h"

String beginHTML() {
  String html = "\
HTTP/1.1 200 OK\n\
Content-Type: text/html\n\
\n\
<!DOCTYPE HTML>\n\
<html>\n\
  <head>\n\
    <meta http-equiv=\"refresh\" content=\"1\" />\n\
  </head>\n\
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
