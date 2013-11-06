# -*- coding: utf-8 -*-

from django.db import models
from django.contrib.auth.models import User
from django.utils.translation import ugettext_lazy as _

from website.models import AnalysePeptide

class DeamonCalculation(models.Model):
    STATES  = [(1,u"Envoyé"),(2,u"Reçu"),(3,u"Déconnecté")]

    deamon          = models.ForeignKey("Deamon")
    analysepeptide  = models.ForeignKey(AnalysePeptide)
    state           = models.IntegerField(_("Status"),choices=STATES)
    send_houre      = models.DateTimeField(_("Envoyé à"))
    recive_houre    = models.DateTimeField(_("Reçu à"))

class Deamon(models.Model):
    ip         = models.IPAddressField(_("IP"))
    port       = models.IntegerField(_("port"))
    owner      = models.ForeignKey(User,blank=True,null=True)
    data_send  = models.ManyToMany(AnalysePeptide,through=DeamonCalculation)
