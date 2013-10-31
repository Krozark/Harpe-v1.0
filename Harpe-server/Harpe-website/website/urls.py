# -*- coding: utf-8 -*-
from django.conf.urls import patterns, include, url

from website.views import *

urlpatterns = patterns('',
       url(r'^$', HomeView.as_view(),name="website-home"), 
       url(r'^a-propos$', HomeView.as_view(),name="website-about"), 
)
