#include "header.hpp"
#include "iostream"
#include "prepareSuggests.hpp"
int main(int argc, char* argv[]) {
  try {
    // Check command line arguments.
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80\n";
      return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc{1};

    preparerSug prepSug =
        preparerSug("/home/rinat/labs/lab-07-http-server/suggestions.json");


    std::thread thrSug([&prepSug]{
      prepSug.serveSuggestions();
    });
    thrSug.detach();
    // here create sugpreferer object
    // call serveSuggestions in thread
    // pointer for sugpreferer obj -> http_server
    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};

    http_server(acceptor, socket,prepSug);

    ioc.run();
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}