# -*- coding: utf-8 -*-

from django.core.urlresolvers import reverse
from django.db import models
from django.db.models.signals import post_delete
from django.utils.translation import ugettext_lazy as _
from django.contrib.auth.models import User

from Kraggne.contrib.gblocks.utils import file_cleanup

# Create your models here.

class AA(models.Model):
    name = models.CharField(_("name"),max_length=255,unique=True)
    slug = models.SlugField(_("short name"),max_length=16)
    mass = models.DecimalField(_("mass"),max_digits=15,decimal_places=10)


    class Meta:
        ordering = ["slug",]
        verbose_name = _("amino acid")


    def __unicode__(self):
        return "%s" % self.slug

class ImpossibleCut(models.Model):
    first = models.ForeignKey(AA,related_name="first")
    second = models.ForeignKey(AA,related_name="second")

    def __unicode__(self):
        return "%s - %s" % (self.first,self.second)

class Enzyme(models.Model):
    name = models.CharField(_("name"),max_length=255,unique=True)
    cut_before = models.ManyToManyField(AA,null=True,blank=True,related_name="AA_before")
    cut_after = models.ManyToManyField(AA,null=True,blank=True,related_name="AA_after")
    cut_imposible = models.ManyToManyField(ImpossibleCut,null=True,blank=True)


    class Meta:
        ordering = ["name",]


    @property
    def direction(self):
        if self.cut_before.all().count() > 1:
            return -1
        if self.cut_after.all().count() > 1:
            return 1
        return 0

    def __unicode__(self):
        return "%s" % self.name

class AnalyseMgf(models.Model):
    owner      = models.ForeignKey(User)
    name       = models.CharField(_("name"),max_length=255,unique=True)
    descriptif = models.TextField(_("Descriptif"),blank=True)
    created    = models.DateTimeField(_("Created"),auto_now=True)
    mgf        = models.FileField(_("MGF"),upload_to="mgf/")

    paginate_by = 3

    class Meta:
        ordering = ["-created",]
        unique_together = ("owner","name")

    
    @property
    def peptides(self):
        return 42

    def __unicode__(self):
        return u"%s" % self.name

    def get_absolute_url(self):
        return reverse("analyse-detail",kwargs={"pk":self.pk})

post_delete.connect(file_cleanup, sender=AnalyseMgf, dispatch_uid="AnalyseMgf.file_cleanup")


