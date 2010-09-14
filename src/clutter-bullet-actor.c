/*
 * Clutter-Bullet
 * Copyright (C) 2010 William Hua
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */



#include <clutter/clutter.h>

#include "clutter-bullet-actor.h"



G_DEFINE_INTERFACE (
  ClutterBulletActor,
  clutter_bullet_actor,
  G_TYPE_INVALID
);



static void
clutter_bullet_actor_default_init (ClutterBulletActorInterface *klass)
{
  static GParamSpec *spec = NULL;

  if (spec == NULL)
  {
    spec = g_param_spec_object ("actor",
                                "Target actor",
                                "Target actor to add to group",
                                CLUTTER_TYPE_ACTOR,
                                (GParamFlags) (G_PARAM_READABLE |
                                               G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property (klass, spec);
  }
}



void
clutter_bullet_actor_bind (ClutterBulletActor *self,
                           ClutterBulletGroup *group)
{
  CLUTTER_BULLET_ACTOR_GET_INTERFACE (self)->bind (self, group);
}



void
clutter_bullet_actor_unbind (ClutterBulletActor *self,
                             ClutterBulletGroup *group)
{
  CLUTTER_BULLET_ACTOR_GET_INTERFACE (self)->unbind (self, group);
}
