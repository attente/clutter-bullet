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

#include "clutter-bullet-card.h"
#include "clutter-bullet-actor.h"

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <btBulletDynamicsCommon.h>

#include "clutter-bullet-private.h"



#define CLUTTER_BULLET_CARD_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_BULLET_TYPE_CARD, ClutterBulletCardPrivate))



struct _ClutterBulletCardPrivate
{
  ClutterActor *actor;

  btRigidBody  *body;

  gdouble       mass;
};



enum
{
  PROP_0,
  PROP_ACTOR,
  PROP_MASS
};



static void clutter_bullet_actor_iface_init  (ClutterBulletActorInterface *iface,
                                              gpointer                     data);

static void clutter_bullet_card_get_property (GObject                     *obj,
                                              guint                        key,
                                              GValue                      *val,
                                              GParamSpec                  *spec);

static void clutter_bullet_card_set_property (GObject                     *obj,
                                              guint                        key,
                                              const GValue                *val,
                                              GParamSpec                  *spec);

static void clutter_bullet_card_bind         (ClutterBulletActor          *self,
                                              ClutterBulletGroup          *group);

static void clutter_bullet_card_unbind       (ClutterBulletActor          *self,
                                              ClutterBulletGroup          *group);



G_DEFINE_TYPE_WITH_CODE (
  ClutterBulletCard,
  clutter_bullet_card,
  CLUTTER_TYPE_ACTOR,
  G_IMPLEMENT_INTERFACE (
    CLUTTER_BULLET_TYPE_ACTOR,
    clutter_bullet_actor_iface_init
  )
);



static void
clutter_bullet_card_init (ClutterBulletCard *self)
{
  self->priv = CLUTTER_BULLET_CARD_GET_PRIVATE (self);

  self->priv->actor = NULL;
  self->priv->body  = NULL;
  self->priv->mass  = 1;
}



static void
clutter_bullet_card_class_init (ClutterBulletCardClass *klass)
{
  GObjectClass *glass = G_OBJECT_CLASS (klass);
  GParamSpec   *spec;

  g_type_class_add_private (klass, sizeof (ClutterBulletCardPrivate));

  glass->get_property = clutter_bullet_card_get_property;
  glass->set_property = clutter_bullet_card_set_property;

  g_object_class_override_property (glass, PROP_ACTOR, "actor");

  spec = g_param_spec_double ("mass",
                              "Object mass",
                              "Mass in kilograms",
                              0,
                              G_MAXDOUBLE,
                              1,
                              (GParamFlags) (G_PARAM_READWRITE |
                                             G_PARAM_CONSTRUCT));

  g_object_class_install_property (glass, PROP_MASS, spec);
}



static void
clutter_bullet_actor_iface_init (ClutterBulletActorInterface *iface,
                                 gpointer                     data)
{
  iface->bind   = clutter_bullet_card_bind;
  iface->unbind = clutter_bullet_card_unbind;
}



static void
clutter_bullet_card_get_property (GObject    *obj,
                                  guint       key,
                                  GValue     *val,
                                  GParamSpec *spec)
{
  ClutterBulletCard *self = CLUTTER_BULLET_CARD (obj);

  switch (key)
  {
    case PROP_ACTOR:
      g_value_set_object (val, self->priv->actor);
      break;

    case PROP_MASS:
      g_value_set_double (val, self->priv->mass);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, key, spec);
      break;
  }
}



static void
clutter_bullet_card_set_property (GObject      *obj,
                                  guint         key,
                                  const GValue *val,
                                  GParamSpec   *spec)
{
  ClutterBulletCard *self = CLUTTER_BULLET_CARD (obj);

  switch (key)
  {
    case PROP_ACTOR:
      self->priv->actor = (ClutterActor *) g_value_get_object (val);
      break;

    case PROP_MASS:
      self->priv->mass = g_value_get_double (val);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, key, spec);
      break;
  }
}



static void
clutter_bullet_card_bind (ClutterBulletActor *self,
                          ClutterBulletGroup *group)
{
  ClutterBulletCard *card = CLUTTER_BULLET_CARD (self);

  btCollisionShape *shape;
  btVector3         tensor;
  gdouble           scale;
  gfloat            w, h;

  g_object_get (group, "scale", &scale, NULL);

  clutter_actor_get_size (card->priv->actor, &w, &h);

  w /= scale;
  h /= scale;

  shape = new btBox2dShape (btVector3 (w / 2, h / 2, 0));
  shape->calculateLocalInertia (card->priv->mass, tensor);

  card->priv->body = new btRigidBody (
    btRigidBody::btRigidBodyConstructionInfo (
      card->priv->mass,
      new ClutterBulletMotionState (card->priv->actor, scale),
      shape,
      tensor
    )
  );
}



static void
clutter_bullet_card_unbind (ClutterBulletActor *self,
                            ClutterBulletGroup *group)
{
  ClutterBulletCard *card = CLUTTER_BULLET_CARD (self);

  if (card->priv->body != NULL)
  {
    delete card->priv->body->getCollisionShape ();
    delete card->priv->body->getMotionState ();
    delete card->priv->body;

    card->priv->body = NULL;
  }
}
