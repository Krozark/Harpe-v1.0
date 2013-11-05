# -*- coding: utf-8 -*-
from django.views.generic.edit import CreateView
from Kraggne.views import GenericViewContextMixin, GenericView, GenericDetailView, GenericFormView, GenericListView #, GenericListFormView, GenericDetailFormView
from Kraggne.contrib.contentblocks.utils import model_to_modelform

from website.models import *
from website.forms import *

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

class AnalyseMgfCreateView(GenericFormView):
    slug          = "analyse-create"
    form_class    = AnalyseMgfForm

    #template_name = "website/analysemgf/create.html"

   # def get_form_class(self):
   #     return model_to_modelform(AnalyseMgf)

   # def get_form_kwargs(self):
   #     kwargs = GenericFormView.get_form_kwargs(self)
   #     kwargs["initial"]["owner"] = self.request.user
   #     return kwargs

   # def form_valid(self,form):
   #     object = form.save(commit=False)
   #     object.owner = self.request.user
   #     object.save()
   #     return super(AnalyseMgfCreateView,self).form_valid(form)
