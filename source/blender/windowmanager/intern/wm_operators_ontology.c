/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2014 Blender Foundation.
 * All rights reserved.
 *
 * 
 * Contributor(s): Vincent Akkermans
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/windowmanager/intern/wm_operators_ontology.c
 *  \ingroup wm
 *
 * Functions for exporting operator type information to RDF.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include <float.h>

//#include <stddef.h>
//#include <assert.h>

//#include "GHOST_C-api.h"

#include "MEM_guardedalloc.h"

#include "DNA_listBase.h"
#include "DNA_windowmanager_types.h"
//#include "DNA_ID.h"
//#include "DNA_object_types.h"
//#include "DNA_screen_types.h"
//#include "DNA_scene_types.h"
//#include "DNA_userdef_types.h"
//#include "DNA_mesh_types.h" /* only for USE_BMESH_SAVE_AS_COMPAT */

//#include "BLF_translation.h"

//#include "PIL_time.h"

#include "BLI_blenlib.h"
#include "BLI_dynstr.h" /*for WM_operator_pystring */
#include "BLI_math.h"
#include "BLI_utildefines.h"
#include "BLI_ghash.h"

//#include "BKE_autoexec.h"
//#include "BKE_blender.h"
//#include "BKE_brush.h"
//#include "BKE_context.h"
//#include "BKE_depsgraph.h"
//#include "BKE_idprop.h"
//#include "BKE_image.h"
//#include "BKE_library.h"
//#include "BKE_global.h"
//#include "BKE_main.h"
//#include "BKE_material.h"
//#include "BKE_report.h"
//#include "BKE_scene.h"
//#include "BKE_screen.h" /* BKE_ST_MAXNAME */
//#include "BKE_utildefines.h"
//
//#include "BKE_idcode.h"

//#include "ED_screen.h"
//#include "ED_util.h"
//#include "ED_object.h"
//#include "ED_view3d.h"

#include "RNA_access.h"
#include "RNA_define.h"
#include "RNA_enum_types.h"

//#include "UI_interface.h"
//#include "UI_resources.h"

#include "WM_api.h"
#include "WM_types.h"

//#include "wm.h"
//#include "wm_draw.h"
//#include "wm_event_system.h"
//#include "wm_event_types.h"
//#include "wm_files.h"
//#include "wm_subwindow.h"
//#include "wm_window.h"


#include <redland.h>
#include <raptor2.h>

#define URI(_name, _uri) static const unsigned char *_name = (const unsigned char *)_uri;

URI(XSD_TRUE, "true")
URI(XSD_FALSE, "false")
URI(XSD_BOOLEAN, "http://www.w3.org/2001/XMLSchema#boolean")

URI(RDF_TYPE, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type")
URI(RDF_PROP, "http://www.w3.org/1999/02/22-rdf-syntax-ns#Property")
URI(RDFS_LABEL, "http://www.w3.org/2000/01/rdf-schema#label")
URI(RDFS_SUBCLASS, "http://www.w3.org/2000/01/rdf-schema#subClassOf")

URI(OWL_CLASS, "http://www.w3.org/2002/07/owl#Class")
URI(OWL_DTPROP, "http://www.w3.org/2002/07/owl#DatatypeProperty")
URI(OWL_OPROP, "http://www.w3.org/2002/07/owl#ObjectProperty")

// OPERATOR
URI(IB_Operator,	"http://rdf.inspectorb.com/operators/Operator")

// FIELDS
URI(IB_hasExec,		"http://rdf.inspectorb.com/operators/fields/hasExec")
URI(IB_hasCheck,	"http://rdf.inspectorb.com/operators/fields/hasCheck")
URI(IB_hasInvoke,	"http://rdf.inspectorb.com/operators/fields/hasInvoke")
URI(IB_hasCancel,	"http://rdf.inspectorb.com/operators/fields/hasCancel")
URI(IB_hasModal,	"http://rdf.inspectorb.com/operators/fields/hasModal")
URI(IB_hasPoll,		"http://rdf.inspectorb.com/operators/fields/hasPoll")
URI(IB_hasUi,		"http://rdf.inspectorb.com/operators/fields/hasUi")

URI(IB_idname,		"http://rdf.inspectorb.com/operators/fields/idname")
URI(IB_name,		"http://rdf.inspectorb.com/operators/fields/name")
URI(IB_description,	"http://rdf.inspectorb.com/operators/fields/description")

URI(IB_hasProperty,	"http://rdf.inspectorb.com/operators/fields/hasProperty")

// PROPERTIES
URI(IB_prop_identifier,		"http://rdf.inspectorb.com/properties/fields/identifier")
URI(IB_prop_name,			"http://rdf.inspectorb.com/properties/fields/name")
URI(IB_prop_description,	"http://rdf.inspectorb.com/properties/fields/description")

URI(IB_Property,			"http://rdf.inspectorb.com/properties/Property");
URI(IB_BooleanProperty,		"http://rdf.inspectorb.com/properties/BooleanProperty");
URI(IB_IntProperty,			"http://rdf.inspectorb.com/properties/IntProperty");
URI(IB_FloatProperty,		"http://rdf.inspectorb.com/properties/FloatProperty");
URI(IB_StringProperty,		"http://rdf.inspectorb.com/properties/StringProperty");
URI(IB_EnumProperty,		"http://rdf.inspectorb.com/properties/EnumProperty");
URI(IB_PointerProperty,		"http://rdf.inspectorb.com/properties/PointerProperty");
URI(IB_CollectionProperty,	"http://rdf.inspectorb.com/properties/CollectionProperty");


static char *ontology_output = NULL;

void WM_operatortypes_ontology_set_output(const char* path)
{
	if (ontology_output)
		MEM_freeN(ontology_output);
	ontology_output = BLI_strdup(path);
}

void WM_operatortypes_ontology_export()
{
	// initialise
	librdf_world *world;
	librdf_storage *storage;
	librdf_model *model;
	librdf_statement *statement;
	raptor_world *raptor_world_ptr = NULL;
	librdf_uri *base_uri;
	librdf_serializer *serializer;
	GHashIterator *op_iter;
	
	FILE *fp;
	
	world = librdf_new_world();
	librdf_world_open(world);
	raptor_world_ptr = librdf_world_get_raptor(world);
	
	storage = librdf_new_storage(world, "memory", "operators", NULL);
	model = librdf_new_model(world, storage, NULL);
	
	base_uri = librdf_new_uri(world, (const unsigned char*)"http://rdf.inspectorb.com/");
	
	// mark properties as data or object properties
	statement = librdf_new_statement(world);
	
	librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, RDF_TYPE));
	librdf_statement_set_object(statement, librdf_new_node_from_uri_string(world, OWL_DTPROP));
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasCancel));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasCheck));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasExec));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasInvoke));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasModal));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasPoll));
	librdf_model_add_statement(model, statement);
	
	librdf_statement_set_subject(statement, librdf_new_node_from_uri_string(world, IB_hasUi));
	librdf_model_add_statement(model, statement);
	
	librdf_free_statement(statement);
	
	
	// describe operators
	op_iter = WM_operatortype_iter();
	for ( ; !BLI_ghashIterator_done(op_iter); BLI_ghashIterator_step(op_iter)) {
		
		wmOperatorType *ot = BLI_ghashIterator_getValue(op_iter);
		librdf_uri *xsdbool = librdf_new_uri(world, XSD_BOOLEAN);
		
		char *ot_node_name = BLI_sprintfN("operators/%s", ot->idname);
		librdf_node *ot_node = librdf_new_node_from_uri_local_name(world, base_uri, (const unsigned char*)ot_node_name);
		
		statement = librdf_new_statement(world);
		
		// mark as an operator
		librdf_statement_set_subject(statement, ot_node);
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, RDF_TYPE));
		librdf_statement_set_object(statement, librdf_new_node_from_uri_string(world, IB_Operator));
		librdf_model_add_statement(model, statement);
		
		// mark as ACTION/ANIM/VIEW3D/etc. operator
		{
			const char *pos = strstr(ot->idname, "_");
			if (pos) {
				int len = pos - ot->idname;
				char *sub = BLI_strdupn(ot->idname, len);
				char *uri = NULL;
				librdf_statement *sc_stat = librdf_new_statement(world);
				
				BLI_ascii_strtolower(sub, len);
				sub[0] = toupper(sub[0]);
				uri = BLI_sprintfN("http://rdf.inspectorb.com/operators/%sOperator", sub);
				
				// mark as special operator
				librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, RDF_TYPE));
				librdf_statement_set_object(statement, librdf_new_node_from_uri_string(world, (const unsigned char*)uri));
				librdf_model_add_statement(model, statement);

				// mark special operator as subclass of operator
				// N.B. this is executed more than necessary, but it prevents having to do
				// it manually.
				librdf_statement_set_subject(sc_stat, librdf_new_node_from_uri_string(world, (const unsigned char*)uri));
				librdf_statement_set_predicate(sc_stat, librdf_new_node_from_uri_string(world, RDFS_SUBCLASS));
				librdf_statement_set_object(sc_stat, librdf_new_node_from_uri_string(world, IB_Operator));
				librdf_model_add_statement(model, sc_stat);
				
				// clean up
				librdf_free_statement(sc_stat);
				
				MEM_freeN(uri);
				MEM_freeN(sub);
			}
		}

		// set name
		if (ot->name && strlen(ot->name)) {
			librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_name));
			librdf_statement_set_object(statement, librdf_new_node_from_literal(world, (const unsigned char*)ot->name, "en", NULL));
			librdf_model_add_statement(model, statement);
		}
		
		// set idname
		if (ot->idname && strlen(ot->idname)) {
			librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_idname));
			librdf_statement_set_object(statement, librdf_new_node_from_literal(world, (const unsigned char*)ot->idname, NULL, NULL));
			librdf_model_add_statement(model, statement);
			
			librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, RDFS_LABEL));
			librdf_model_add_statement(model, statement);
		}
		
		// description
		if (ot->description && strlen(ot->description)) {
			librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_description));
			librdf_statement_set_object(statement, librdf_new_node_from_literal(world, (const unsigned char*)ot->description, "en", NULL));
			librdf_model_add_statement(model, statement);
		}
		
		// hasExec
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasExec));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->exec == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasCheck
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasCheck));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->check == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasInvoke
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasInvoke));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->invoke == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasCancel
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasCancel));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->cancel == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasModal
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasModal));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->modal == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasPoll
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasPoll));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->poll == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// hasUi
		librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasUi));
		librdf_statement_set_object(statement, librdf_new_node_from_typed_literal(world, ot->ui == NULL ? XSD_FALSE : XSD_TRUE, NULL, xsdbool));
		librdf_model_add_statement(model, statement);
		
		// properties
		if (ot->srna) {
			for(Link *link = (Link*)RNA_struct_type_properties(ot->srna)->first;
				link;
				link=link->next) {

				PropertyRNA *prop = (PropertyRNA*)link;
				
				printf("%s\n", RNA_property_identifier(prop));
				
				librdf_statement *pstat = librdf_new_statement(world);
				
				// create parameter
				librdf_node *parameter = librdf_new_node(world);
				
				// set as parameter of the operator
				librdf_statement_set_predicate(statement, librdf_new_node_from_uri_string(world, IB_hasProperty));
				librdf_statement_set_object(statement, parameter);
				librdf_model_add_statement(model, statement);
				
				// set subject for statement to the blank parameter node
				librdf_statement_set_subject(pstat, parameter);
				
				// set as Property
				librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, RDF_TYPE));
				librdf_statement_set_object(pstat, librdf_new_node_from_uri_string(world, IB_Property));
				librdf_model_add_statement(model, pstat);
				
				// set identifier
				if (RNA_property_identifier(prop) && strlen(RNA_property_identifier(prop))) {
					librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, IB_prop_identifier));
					librdf_statement_set_object(pstat, librdf_new_node_from_literal(world, (const unsigned char*)RNA_property_identifier(prop), NULL, NULL));
					librdf_model_add_statement(model, pstat);
					
					librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, RDFS_LABEL));
					librdf_model_add_statement(model, pstat);
				}
				
				// set name
				if (RNA_property_ui_name(prop) && strlen(RNA_property_ui_name(prop))) {
					librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, IB_prop_name));
					librdf_statement_set_object(pstat, librdf_new_node_from_literal(world, (const unsigned char*)RNA_property_ui_name(prop), "en", NULL));
					librdf_model_add_statement(model, pstat);
				}
				
				// set description
				if (RNA_property_ui_description(prop) && strlen(RNA_property_ui_description(prop))) {
					librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, IB_prop_description));
					librdf_statement_set_object(pstat, librdf_new_node_from_literal(world, (const unsigned char*)RNA_property_ui_description(prop), "en", NULL));
					librdf_model_add_statement(model, pstat);
				}
				
				const unsigned char *ptype = NULL;
								
				switch (RNA_property_type(prop)) {
					case PROP_BOOLEAN: ptype = IB_BooleanProperty; break;
					case PROP_INT: ptype = IB_IntProperty; break;
					case PROP_FLOAT: ptype = IB_FloatProperty; break;
					case PROP_STRING: ptype = IB_StringProperty; break;
					case PROP_ENUM: ptype = IB_EnumProperty; break;
					case PROP_POINTER: ptype = IB_PointerProperty; break;
					case PROP_COLLECTION: ptype = IB_CollectionProperty; break;
					default: break;
				}
				
				// set as Property
				if (ptype) {
					librdf_statement_set_predicate(pstat, librdf_new_node_from_uri_string(world, RDF_TYPE));
					librdf_statement_set_object(pstat, librdf_new_node_from_uri_string(world, ptype));
					librdf_model_add_statement(model, pstat);
				}
				
				// clean up
				librdf_free_statement(pstat);
			}
		}
		
		// TODO: flags
		
		// clean up
		MEM_freeN(ot_node_name);
		librdf_free_statement(statement);
	}
	
	// write to file
	fp = fopen(ontology_output, "w");
	serializer = librdf_new_serializer(world, "turtle", NULL, NULL);
	librdf_serializer_serialize_model_to_file_handle(serializer, fp, base_uri, model);
	fclose(fp);
	
	// clean up
	librdf_free_serializer(serializer);
	librdf_free_uri(base_uri);

	librdf_free_model(model);
	librdf_free_storage(storage);
	
	librdf_free_world(world);
	
	BLI_ghashIterator_free(op_iter);
	
	MEM_freeN(ontology_output);
	ontology_output = NULL;
}

bool WM_operatortypes_ontology_should_export()
{
	return (ontology_output != NULL);
}