add to Fiddler's CustomScript.js for access to service:

    static function OnBeforeResponse(oSession: Session) {
        // ...
        oSession.oResponse.headers["Access-Control-Allow-Origin"] = "*";
    }
