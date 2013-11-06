# -*- coding: utf-8 -*-

from django.views.generic import ListView, TemplateView, DetailView
from harpe_communication.models import *


class HomeView(TemplateView):
    template_name = 'harpe_communication/home.html'
