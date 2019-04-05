#include<boost/asio.hpp> // boost 와 관련된 lib 파일을 사용할 수 있게 해준다
#include<iostream>

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main(){
    boost::asio::io_service io_service; //클라이언트 접속 준비
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),PORT_NUMBER);//Ipv4 포트 번호 사용
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);//클라이언트 접속 받아드림 , io_service 와 endpoint 를 인자로 사용
    boost::asio::ip::tcp::socket socket(io_service);//클라이언트에 할당할 tcp::socket 만든다
    acceptor.accept(socket);
    std::cout<< "클라이언트 접속"<< std::endl;
    
    for(;;)
    {
        std::array<char, 128> buf;
        buf.assign(0);
        
        boost::system::error_code error;//랩핑한 클래스로 에러 발생시 에러코드와 에러 메시지를 얻어올수 있다
        size_t len = socket.read_some(boost::asio::buffer(buf),error);//socket 클래스의 read_some 멤버함수를 사용하여 클라이언트가 보낸 데이터를 받는다
        //동기형 방식이라 데이터가 다 들어올때까지 대기상태에 있다
        if (error){//에러 발생시 접속이 끊어져서 에러나는건지 그 이외에 에러인지 확인이 필요
            if(error == boost::asio::error::eof){
                std::cout<<"클라이언트와 연결이 끊어졌습니다"<<std::endl;
            }
            else{
                std::cout<<"error No : "<<error.value()<<"error Message:"<<error.message()<<std::endl;
            }
            break;
        }
        
        std::cout<<"클라이언트에서 받은 메시지 : "<<&buf[0]<<std::endl;
        char szMessage[128] = {0,};
        sprintf_s(szMessage, 128-1,"Re:%s",&buf[0]);
        int nMsgLen = strnlen_s(szMessage, 128-1);
        
        boost::system::error_code ignored_error;
        socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);
        std::cout<<"클라이언트에 보낸 메시지: "<<szMessage<<std::endl;
        
    }
    getchar();
    return 0;
}
