from django.core.management.base import BaseCommand, CommandError
from website.contrib.commuication.utils import Socket, create_socket

class Command(BaseCommand):

    def handle(self, *args, **options):
        sock = create_socket()
        if not sock:
            return
        print "Ready"
        print "Call function with id 1 (get version)"
        print sock.call('i',1)

        print "Call function with id 2 (testParamInt)"
        print sock.call('i',2,"i",25)

