# -*- coding: utf-8 -*-

from Kraggne.views import GenericView, GenericDetailView, GenericFormView, GenericListView #, GenericListFormView, GenericDetailFormView

from website.models import *


class HomeView(GenericView):
    slug          = "accueil"
    template_name = "website/home.html"



class AnalyseMgfView(GenericListView):
    model       = AnalyseMgf
    slug        = "mes-analyses"

    def get_queryset(self):
        return AnalyseMgf.objects.filter(owner=self.request.user)
