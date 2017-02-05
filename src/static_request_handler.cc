#include <fstream>
#include <sstream>
#include <string>
#include "static_request_handler.h"

namespace http {
namespace server {


StaticRequestHandler::StaticRequestHandler(const std::vector<std::string>& serve_paths_,
                                           const std::map<std::string, std::string>& uri_root2base_dir_): 
  RequestHandler(serve_paths_),
  uri_root2base_dir(uri_root2base_dir_)
{
}

std::string
StaticRequestHandler::extension2type(std::string extension) {
  if (extension == "gif") return "image/gif";
  else if (extension == "htm") return "text/html";
  else if (extension == "html") return "text/html";
  else if (extension == "jpg") return "image/jpeg";
  else if (extension == "png") return "image/png";
  else return "text/plain";
}

void 
StaticRequestHandler::handle_request(const request& req, reply& rep) {
  std::string request_uri = req.uri;
  if (request_uri[request_uri.size() - 1] == '/') {
    request_uri += "index.html";
  }

  std::string full_path;
  // Request path must be absolute and not contain "..".
  for (auto it = uri_root2base_dir.begin(); it != uri_root2base_dir.end(); ++it) {
    std::string uri_root = it->first;
    if (request_uri.find(uri_root) == 0) {
      // invalid if open a directory 
      if (request_uri.size() == uri_root.size()) {
        rep = reply::stock_reply(reply::not_found);
        return;
      }
      full_path = it->second + request_uri.substr(uri_root.size());
      break;
    }
  }
  
  // Determine the file extension.
  std::size_t last_slash_pos = request_uri.find_last_of("/");
  std::size_t last_dot_pos = request_uri.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_uri.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());

  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = extension2type(extension);
}


} // namespace server
} // namespace http