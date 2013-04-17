# -*- coding: utf-8 -*-

HARPE_DB = "Harpe-data"

class HarpeRouter(object):

    def db_for_read(self, model, **hints):
        if model._meta.app_label == HARPE_DB :
            return HARPE_DB
        return None

    def db_for_write(self, model, **hints):
        if model._meta.app_label == HARPE_DB:
            return HARPE_DB
        return None

    def allow_relation(self, obj1, obj2, **hints):
        if obj1._meta.app_label == HARPE_DB or obj2._meta.app_label == HARPE_DB:
            return True
        return None

    def allow_syncdb(self, db, model):
        if db == HARPE_DB:
            return model._meta.app_label == HARPE_DB
        elif model._meta.app_label == HARPE_DB:
            return False
        return None
