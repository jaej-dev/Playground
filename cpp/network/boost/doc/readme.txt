Network framework vi boost asio


1. 비동기 통신에서 고민해야할 두가지 (Network framework에서 처리해야할 두가지)
패킷이 서버로 전송된 이후 서버가 언제 응답할지 알 수 없음. (티이밍의 문제)
패킷이 서버로 전송된 이후 서버가 어떻게 응답할지 알 수 없음.  (버퍼 크기의 문제)

2. asio 기본 요소

asio::error_code ec;
-> 12가지 오류 발생시 전달될 코드

asio:io_context context;
-> network 작업을 수행할 공간

asio::io_context::work idleWork(context); 
-> context가 종료되지 않도록 임시로 task를 할당함 (일반적인 루틴이 아니라 강의를 위해 사용한듯)

std::thread thrContext = std::thread([&]() { context.run(); });
-> main thread 의 실행을 차단하지 않기 위해 thread 생성하여 작업 공간을 할당하여 사용한다.

asio:ip::tcp::endpoint endpoint(asio::ip::make_address(“127.0.0.1”, ec), 80);
->  tcp 스타일의 연결할 주소로서 ip address 와 port로 구성됨

asio::ip::tcp::socket socket(context);
->  network의 입출구 역할을 하며 context를 인자로 주어야 함. server의 경우 자체 socket을 사용하지 않고 asio 내부에 숨겨져 있다. 그러므로 socket을 가져오기 위해 asio::ip::tcp::acceptor 를 사용한다.

socket.connect(endpoint, ec); 
-> 할당했던 주소로 연결시도

asio::ip::resolver (context);
asio::ip::resolver::query query(“www.boost.org”, “http”);
-> 호스트와 서비스 이름을 조회하여 asio::ip::tcp::endpoint 로 변환된 값을 받을수 있다.


if (context.is_open())  {
  GrabSomeData(socket);

  std::string sRequest = “GET /index.html HTTP/1.1\r\n” ...;

  socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
  
  using namespace std::chrono_literals;
  std::this_thread::sleep_for20000ms);

  context.stop();
  if (thrContext.joinable()) thrcontext.join();
}

std::vector<char> vBuffer(1 * 1024); // 1KB씩 읽기 위해
void GrabSomeData(asio::ip::tcp::socket& socket) {
  socket.async_read_some(asio::buffer(vBuffer.data(). vBuffer.size()), 
    [&](std::error_code ec, std::size_t length) {
        if (!ec) {
          std::cout << “\n\nRead ” << length << “ bytes\n\n”;
          for (int i = 0; i < length; i++) {
            std::cout << vBuffer[i];
          }
        GrabSomeData(socket); // 재귀 호출
        }
    });
}


2. Network Framework 
2.1. Architecture


2.1.1. Server 



server는 한개로 고유해야 한다. (늘 자신의 주소가 존재한다.)
클라이언트가 언제든지 접속하거나, 접속 후 끈을수 있으므로 이에대한 처리가 굉장이 중요하다.
asio context의 accept connection이 백그라운드에서 이를 수행한다. 클라이언트가 socket을 통해 연결되면  connection 인스턴스를 하나 생성한다. 만약 서버가 여러가지 이유로 클라이언트의 접속을 거부할수 있으므로 reject 인스턴트도 필요하다.
client 다수를 연결할 수 있다.
client 간 정보 교환을 허용 한다.
client들 에게서 받은 메세지를 큐에서 관리해야 한다. 또한 특정 client 혹은 전체 client에게 메세지를 전달할수 있어야 한다.
server에는 socket이 


2.1.2. Client (net_client.h / class client_interface)
client 는 다수가 될수 있다.
io_context, socket 인스턴스를 만들고 connection 클래스에개 asio를 넘긴다.
서버에서 전송한 메세지를 큐에서 관리해야 한다.



2.1.3. Connection (net_connection.h)
server 및 client 공통으로 사용되므로 필요한 기능 모두 이곳에 구현
server에서 보내는 메세지를 connection 내부 큐에서 보관했다가 client로 보낸다.
client에서 보내는 메세지를 connection 내부 큐에서 보관했다가 server로 보낸다.
ConnectToServer 
Disconnect 
IsConnected 
Send 
데이터의 송수신 입출구이므로 socket이 필요함. 
network 작업 공간이 필요하므로 io_context이 필요함.
메세지를 보내야 하므로 thread safe queue가 필요함.
client를 구분해야 하므로 관리한 thread safe queue가 필요함.

2.2. thread safe queue
병렬 산발적인 요청에 따라 critical section이 발생되므로 thread safe queue가 필요함.
std::mutex, std::dequeue, std::scoped_lock lock(mutex_); 로 구현


(
