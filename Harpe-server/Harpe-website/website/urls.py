# -*- coding: utf-8 -*-
from django.conf.urls import patterns, include, url

from website.views import *

urlpatterns = patterns('',
       url(r'^$', HomeView.as_view(),name="website-home"), 
       url(r'^analyses/list/$', AnalyseMgfListView.as_view(),name="analyse-list"), 
       url(r'^analyse/(?P<pk>[\d]+)/$', AnalyseMgfDetailView.as_view(),name="analyse-detail"), 
       url(r'^analyse/create/$', AnalyseMgfCreateView.as_view(),name="analyse-create"), 
)
