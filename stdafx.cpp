#include "Markup.cpp"
#include "include/httplib.h"
#include <iostream>
/*

A C++11 single-file header-only cross platform HTTP/HTTPS library.

It's extremely easy to setup. Just include httplib.h file in your code!

see https://github.com/yhirose/cpp-httplib

markup document see http://www.firstobject.com/dn_markup.htm

$ sudo g++ -std=c++11 -D_DEBUG -g -o stdafx stdafx.cpp

*/
int main(void)
{
    httplib::Client cli("192.168.2.9", 8001);
    char s[]=("<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Body><IBMGetWorkBillInfoByLsbh xmlns=\"http://tempuri.org/\"><lsbh>18828947</lsbh></IBMGetWorkBillInfoByLsbh></soap:Body></soap:Envelope>");
    auto res = cli.Post("/Service1.asmx",s,"text/xml");
    std::string xmlStr;
    if(res && res->status==200){
        xmlStr =res->body;
        CMarkup xml;
        xml.SetDoc((const char *)xmlStr.data());
        xml.ResetMainPos();
        if(xml.FindElem("soap:Envelope")){//
            xml.IntoElem();
            if(xml.FindElem("soap:Body")){
                xml.IntoElem();
                if(xml.FindElem("IBMGetWorkBillInfoByLsbhResponse")){
                    xml.IntoElem();
                     if(xml.FindElem("IBMGetWorkBillInfoByLsbhResult")){
                            std::cout << xml.GetData()<<std::endl;
                     }
                }
            }
            xml.OutOfElem();
        }else{
            std::cout<<"null"<<std::endl;
        }
    }else{
        std::cout << "not ok" << std::endl;
    }
}
