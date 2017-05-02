
#include "avahi.hpp"

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/error.h>
#include <assert.h>
#include <stdio.h>

#include <syslog.h>

static AvahiEntryGroup *avahiGroup = NULL;

void AvahiGroupCallback(AvahiEntryGroup *_avahiGroup, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata)
{
	assert(_avahiGroup == avahiGroup || avahiGroup == NULL);
	avahiGroup = _avahiGroup;
	switch (state)
	{
		case AVAHI_ENTRY_GROUP_ESTABLISHED:
			{
				syslog(LOG_INFO, "avahi successfully established");
			}
			break;
		case AVAHI_ENTRY_GROUP_COLLISION:
		case AVAHI_ENTRY_GROUP_FAILURE:
			{
				syslog(LOG_ERR, "avahi entry group failure: %s", avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(avahiGroup))));
			}
			break;
		case AVAHI_ENTRY_GROUP_UNCOMMITED:
		case AVAHI_ENTRY_GROUP_REGISTERING:
			{
			}
			break;
	}
	return;
}

void AvahiCreateService(AvahiClient *avahiClient)
{
avahiClient = NULL;
	return;
}



void StartAvahi()
{


}
