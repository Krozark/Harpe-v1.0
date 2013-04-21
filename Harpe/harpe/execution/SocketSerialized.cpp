#include "SocketSerialized.hpp"

namespace ntw {

SocketSerialized::SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole): Serializer(255), Socket(dommaine,type,protocole)
{
    //reserver les 2 premier bits pour la taille
    _cursor_end =_cursor_begin = 2;
    is_send=false;

};

void SocketSerialized::Send()
{
    //écrire la taille dans les 2 primer bits
    //envoyer
};

void SocketSerialized::Receive()
{
    //recuperer la taille dans les 2 premier oct
    //remplacer le buffer
};

void SocketSerialized::clear()
{
    _cursor_begin = _cursor_end = 2;
}

    

};
