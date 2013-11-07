# -*- coding: utf-8 -*-


from django.http import HttpResponse
from django.utils import simplejson as json
from django.views.generic.edit import CreateView, FormView
from django.views.generic.base import TemplateResponseMixin, View
from Kraggne.views import GenericViewContextMixin, GenericView, GenericDetailView, GenericFormView, GenericListView #, GenericListFormView, GenericDetailFormView
from Kraggne.contrib.contentblocks.utils import model_to_modelform

from website.models import *
from website.forms import *

####################### MIXIN / Base class #######################"
class JSONResponseMixin(object):
    """
    A mixin that can be used to render a JSON response.
    """
    def __init__(self,*args,**kwargs):
        self.status = "ok"
        self.message = ""
        self.data = {}

    def error(self,msg):
        self.status = "error"
        self.message = msg
        return self.render_to_json_response()

    def render_to_json_response(self,**response_kwargs):
        """
        Returns a JSON response, transforming 'context' to make the payload.
        """
        return HttpResponse(
            self.convert_context_to_json(
                {"status" : self.status,
                 "message" : self.message,
                 "data" : self.data,
                }
            ),
            content_type='application/json',
            **response_kwargs
        )

    def convert_context_to_json(self, context):
        "Convert the context dictionary into a JSON object"
        # Note: This is *EXTREMELY* naive; in reality, you'll need
        # to do much more complex handling to ensure that arbitrary
        # objects -- such as Django model instances or querysets
        # -- can be serialized as JSON.
        return json.dumps(context)

class JSONAjaxView(JSONResponseMixin,View):
    def render_to_response(self, context, **response_kwargs):
        return self.render_to_json_response(**response_kwargs)

    def get(self,request, *args, **kwargs):
        self.error("get not accepted")
        return self.render_to_response(None)

    def post(self,request, *args, **kwargs):
        return self.render_to_response(None)

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

from website.contrib.communication.utils import send_AnalyseMgf_to_calc
class AnalyseAjaxRecieverView(JSONAjaxView):

    def post(self,request, *args, **kwargs):
        obj = AnalyseMgf.objects.filter(pk=kwargs["pk"])[:1]
        if not obj:
            return self.error("Analyse pk is not valid")

        obj = obj[0]

        if obj.owner != request.user:
            return self.error("Analyse pk is not valid")

        if not send_AnalyseMgf_to_calc(obj):
            return self.error("An error accure whene trying to send calculation")

        return super(AnalyseAjaxRecieverView,self).post(request,*args,**kwargs)

#class AnalyseMgfAnalyse(GenericDetailView):
#    model       = AnalyseMgf
#    slug        = "analyse-analyse"
#
#    def get_context_data(self,**kwargs):
#        context = super(AnalyseMgfAnalyse,self).get_context_data(**kwargs)
#        if context["object"].owner != self.request.user:
#            context["object"] = None
#
#        return context

class AnalyseMgfCreateView(GenericFormView):
    slug          = "analyse-create"
    form_class    = AnalyseMgfForm

