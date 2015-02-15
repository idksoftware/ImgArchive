/*
 * XMLReader.h
 *
 *  Created on: May 7, 2014
 *      Author: wzw7yn
 */

#ifndef XMLREADER_H_
#define XMLREADER_H_
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/framework/LocalFileFormatTarget.hpp>
namespace simplearchive {

class XMLReader {


public:
	XMLReader();
	virtual ~XMLReader();
};

} /* namespace simplearchive */
#endif /* XMLREADER_H_ */

/*
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include "Converter.h"

using namespace xercesc;
using namespace std;

//------------------------------
/// @namespace JETPLAN_CONVERTER
//------------------------------
namespace JETPLAN_CONVERTER
{
  typedef struct
  {
    const char * name;
    const char * value;
  } attribute_t;

  class XMLWriter
  {
  private:

    DOMElement* saveElement (const char *elem);

    // -------------------------------------------------
    // This is the Xerces source and target mission file.
    // Possibly will eventually be the same file.
    // -------------------------------------------------
    LocalFileFormatTarget * XMLSourceFile;
    LocalFileFormatTarget * XMLTargetFile;

    DOMImplementation* m_impl;
    DOMLSParser*       m_parser;
    DOMDocument*       m_doc;
    DOMNode*           m_taskElement;
    DOMNode*           m_waypointElement;

  public:
    XMLWriter ();
    ~XMLWriter ();

    ErrorState addNode (const char * base_element, const char * element,
        const char* element_value, attribute_t* att1 = NULL, attribute_t* att2 =
            NULL, attribute_t* att3 = NULL);

    ErrorState addElement (const char* base_element,
        const char * new_element);

    // ------------------------------------------------------------------------------
    // A more generalised mechanism for dealing with elements with multiple
    // instances distinguished by an id should be found.
    // But currently waypoints and tasks are dealt with individually.
    //
    // WAYPOINTS
    //
    // Note that waypoints do not have a waypoint_id field. The closest to this
    // is <waypoint_label> but this is not a required field (although it might
    // be for waypoints constructed from Jetplan.
    // --------------------------------------------------------------------------------

    ErrorState addWaypoint (int waypoint_id);

    ErrorState updateWaypoint (int waypoint_id, const char * waypoint_tag,
        const char * data, attribute_t* att1 = NULL, attribute_t* att2 = NULL,
        attribute_t* att3 = NULL);

    // --------------------------------------------------------------------------------
    // TASKS
    // Note that tasks have a task_id field which is being used here as the index.
    // However the <task_id> element is strictly an alphanumeric, although we are
    // using it as an integer index value. (Overloaded the function to allow for
    // alpharnumeric id strings
    // --------------------------------------------------------------------------------

    ErrorState addTask (const int task_id);
    ErrorState addTask (const char * task_id);

    ErrorState updateTask (const int task_id, const char * task_tag,
        const char * data, attribute_t* att1 = NULL, attribute_t* att2 = NULL,
        attribute_t* att3 = NULL);
    ErrorState updateTask (const char * task_id, const char * task_tag,
        const char * data, attribute_t* att1 = NULL, attribute_t* att2 = NULL,
        attribute_t* att3 = NULL);


    // --------------------------------------------------------------------------------
    // The default XML file holds an outline mission structure. The <task> and <waypoint>
    // elements from this file will be used as the basis for any elements created by the
    // operations above. If more than one such element exists then the first instance
    // will be used. All existing <task> and <waypoint> elements will be removed from the
    // default structure prior to writing out, with any new elements defined by these
    // operations replacing them.
    // --------------------------------------------------------------------------------

    ErrorState getDefaultXML (const char * filename);
    ErrorState createXMLDoc ();

    ErrorState putMissionXML (const char * filename);

  };
}

#endif // XMLWRITERH
 */
