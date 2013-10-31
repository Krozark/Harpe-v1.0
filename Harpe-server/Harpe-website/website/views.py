# -*- coding: utf-8 -*-

from Kraggne.views import GenericView


class HomeView(GenericView):
    slug          = "accueil"
    template_name = "website/home.html"
