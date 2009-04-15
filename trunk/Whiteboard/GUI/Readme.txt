This is mostly with regard to the networking.

The RakNet library was fine, but wxWidgets uses an event-based model for handling activities, and RakNet didn't fit in to that model.  wxWidgets comes with it's own networking library, so I just used that instead.

As far as current functionallity goes, a client can connect to a server, and the two can exchange text.  Multiple chat windows with one server doesn't work yet.