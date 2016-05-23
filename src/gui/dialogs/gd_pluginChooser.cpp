/* -----------------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * gd_pluginChooser
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2016 Giovanni A. Zuliani | Monocasual
 *
 * This file is part of Giada - Your Hardcore Loopmachine.
 *
 * Giada - Your Hardcore Loopmachine is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Giada - Your Hardcore Loopmachine is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Giada - Your Hardcore Loopmachine. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------- */


#ifdef WITH_VST


#include "../../utils/gui_utils.h"
#include "../../core/channel.h"
#include "../../core/mixer.h"
#include "../../core/conf.h"
#include "../../core/pluginHost.h"
#include "../elems/ge_pluginBrowser.h"
#include "../elems/ge_mixed.h"
#include "gd_pluginChooser.h"


extern PluginHost G_PluginHost;
extern Mixer      G_Mixer;
extern Conf       G_Conf;


gdPluginChooser::gdPluginChooser(int X, int Y, int W, int H, int stackType, class Channel *ch)
  : gWindow(X, Y, W, H, "Available plugins"), ch(ch), stackType(stackType)
{
  /* top area */
  Fl_Group *group_top = new Fl_Group(8, 8, w()-16, 20);
  sortMethod = new gChoice(group_top->x() + 45, group_top->y(), 100, 20, "Sort by");
    gBox *b1 = new gBox(sortMethod->x()+sortMethod->w(), group_top->y(), 100, 20); 	// spacer window border <-> menu
  group_top->resizable(b1);
  group_top->end();

  /* center browser */
  browser = new gePluginBrowser(8, 36, w()-16, h()-70);

  /* ok/cancel buttons */
  Fl_Group *group_btn = new Fl_Group(8, browser->y()+browser->h()+8, w()-16, h()-browser->h()-16);
    gBox *b2 = new gBox(8, browser->y()+browser->h(), 100, 20); 	// spacer window border <-> buttons
    addBtn = new gClick(w()-88, group_btn->y(), 80, 20, "Add");
    cancelBtn = new gClick(addBtn->x()-88, group_btn->y(), 80, 20, "Cancel");
  group_btn->resizable(b2);
  group_btn->end();

  end();

  sortMethod->add("Name");
  sortMethod->add("Category");
  sortMethod->add("Manufacturer");
  sortMethod->callback(cb_sort, (void*) this);
  sortMethod->value(G_Conf.pluginSortMethod);

  addBtn->callback(cb_add, (void*) this);
  addBtn->shortcut(FL_Enter);
  cancelBtn->callback(cb_close, (void*) this);

  resizable(browser);
	gu_setFavicon(this);
  show();
}


/* -------------------------------------------------------------------------- */


gdPluginChooser::~gdPluginChooser()
{
  G_Conf.pluginChooserX = x();
  G_Conf.pluginChooserY = y();
  G_Conf.pluginChooserW = w();
  G_Conf.pluginChooserH = h();
  G_Conf.pluginSortMethod = sortMethod->value();
}


/* -------------------------------------------------------------------------- */


void gdPluginChooser::cb_close(Fl_Widget *v, void *p) { ((gdPluginChooser*)p)->__cb_close(); }
void gdPluginChooser::cb_add(Fl_Widget *v, void *p)   { ((gdPluginChooser*)p)->__cb_add(); }
void gdPluginChooser::cb_sort(Fl_Widget *v, void *p)  { ((gdPluginChooser*)p)->__cb_sort(); }


/* -------------------------------------------------------------------------- */


void gdPluginChooser::__cb_close() {
	do_callback();
}


/* -------------------------------------------------------------------------- */


void gdPluginChooser::__cb_sort() {
	G_PluginHost.sortPlugins(sortMethod->value());
  browser->refresh();
}


/* -------------------------------------------------------------------------- */


void gdPluginChooser::__cb_add() {
  int index = browser->value() - 3; // subtract header lines
  if (index >= 0 && index < G_PluginHost.countAvailablePlugins()) {
    G_PluginHost.addPlugin(index, stackType, &G_Mixer.mutex_plugins, ch);
    do_callback();
  }
}

#endif // #ifdef WITH_VST
