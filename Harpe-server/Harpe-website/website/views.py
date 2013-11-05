# -*- coding: utf-8 -*-

from Kraggne.views import GenericView, GenericDetailView, GenericFormView, GenericListView #, GenericListFormView, GenericDetailFormView

from website.models import *

################## General ############################

class HomeView(GenericView):
    slug          = "accueil"
    template_name = "website/home.html"


#################" AnalyseMgf #########################
class AnalyseMgfListView(GenericListView):
    model       = AnalyseMgf
    slug        = "analyse-list"

    def get_queryset(self):
        return AnalyseMgf.objects.filter(owner=self.request.user)

class AnalyseMgfDetailView(GenericDetailView):
    model       = AnalyseMgf
    slug        = "analyse-detail"

    def get_context_data(self,**kwargs):
        context = super(AnalyseMgfDetailView,self).get_context_data(**kwargs)
        if context["object"].owner != self.request.user:
            context["object"] = None

        return context
