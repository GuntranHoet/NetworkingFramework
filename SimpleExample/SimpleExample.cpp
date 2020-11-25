#include <chrono>
#include <iostream>

// Windows only problem: "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately."
// Each version of windows has a slightly different way of handling networking.
// This simply specifies which version to use.
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (int i = 0; i < length; i++)
				{
					std::cout << vBuffer[i];
				}

				GrabSomeData(socket);
			}
		}
	);
}

int main()
{
	asio::error_code ec;

	// Create a "context" - essenctially the platform specific interface
	asio::io_context context;

	// Give some fake tasks to asio so the context doesn't finish
	asio::io_context::work idleWork(context);

	// Start the context - run in its own thread so that if it has to wait it doesn't block the main program execution
	std::thread thrContext = std::thread([&]() { context.run(); });

	// Get the address of somewhere we wish to connect to
	//asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80); // example.com (http)
	//asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 80); // this local machine
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80); // larget website to connect to

	// Create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	// Tell socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{
		// Prime the asio context with an instruction to read some data
		GrabSomeData(socket);

		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);


		// Program does something else, while asio handles data transfer in the backgroudn
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2000ms); //20s
	}

	system("pause");
	return 0;
}
