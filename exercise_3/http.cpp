#include "http.h"

String query(String request, String variable) {
  int begin = request.indexOf(variable + "=");

  if (begin < 0) return "";

  int end = request.indexOf("&", begin);
  if (end < 0) end = request.indexOf(" ", begin);

  return request.substring(begin + 2, end);
}

