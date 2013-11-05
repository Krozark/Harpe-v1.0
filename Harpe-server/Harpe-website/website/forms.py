# -*- coding: utf-8 -*-

from django import forms
from website.models import *


class AnalyseMgfForm(forms.ModelForm):
    class Meta:
        model = AnalyseMgf
        exclude = ["owner",]

    def clean_mgf(self):
        return self.cleaned_data["mgf"]

    def save(self,commit=True,request=None):
        object = super(forms.ModelForm,self).save(commit=False)
        object.owner = request.user

        if commit :
            object.save()
        return object
