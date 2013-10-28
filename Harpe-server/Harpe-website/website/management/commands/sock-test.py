from django.core.management.base import BaseCommand, CommandError
from website.utils import Socket

class Command(BaseCommand):

    def handle(self, *args, **options):
        sock = Socket(Socket.Dommaine.IP,Socket.Type.TCP)
        sock.connect("127.0.0.1",3987)
        if sock.verify_connexion() != sock.NTW_ERROR_NO:
            print "An error accur"
            return
        print "Ready"
        print "Call function with id 1 (get version)"
        print sock.call('i',1)

        print "Call function with id 2 (testParamInt)"
        print sock.call('i',2,"i",25)

