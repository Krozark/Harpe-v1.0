# -*- coding: utf-8 -*-

from django.contrib import admin
from website.models import *

class AAAdmin(admin.ModelAdmin):
    list_display = ("name","slug","mass")
    search_list = ("name","slug")
    prepopulated_fields = {'slug':('name',)}
admin.site.register(AA,AAAdmin)

class ImpossibleCutAdmin(admin.ModelAdmin):
    list_display = ("first","second")
    search_list = ("first","second")
    list_filter = ("first","second")
admin.site.register(ImpossibleCut,ImpossibleCutAdmin)

class EnzymeAdmin(admin.ModelAdmin):
    list_display = ("name",)
    search_list = ("name",)
    filter_horizontal = ("cut_before","cut_after","cut_imposible")
admin.site.register(Enzyme,EnzymeAdmin)


class AnalyseMgfAdmin(admin.ModelAdmin):
    list_display = ("name","owner","created","mgf")
    search_list = ("name","owner")
admin.site.register(AnalyseMgf,AnalyseMgfAdmin)
