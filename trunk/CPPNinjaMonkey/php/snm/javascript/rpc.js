var RPCCall = Class.create(Ajax.Request, {
    initialize: function($super, id, name, args, callback, manager) {
        this.id = id;
        this.name = name;
        this.args = args;
        this.callback = callback;
        this.manager = manager;
        
        var thisRPCCall = this;
        var encodedArgs = Object.toJSON(args);
        if (!encodedArgs) {
            try {
                this.callback(null, 'Unable to JSON-encode arguments: ' + args);
            } finally {
                this.manager.endCall(id);
            }
            return;
        }
        
        $super('ajax.php', { 
            method: 'post', 
            parameters: { 
                name: this.name, 
                args: encodedArgs
            },
            onSuccess: function(response) { 
                thisRPCCall.onSuccess(response); 
            },
            onFailure: function(response) { 
                thisRPCCall.onFailure(response); 
            }
        });
    },
    
    onSuccess: function(response) {
        try {
            if (response.responseText) {
                try {
                    // WARNING WARNING WARNING: Does not protect against HIJACKING!
                    // TODO: Add hijacking protection.
                    var sanitize = true;
                    var result = response.responseText.evalJSON(sanitize);
                    this.callback(result, null);
                } catch (e) {
                    this.callback(null, 'Response text: "' + response.responseText + '", failed decoding: ' + e);
                }
            } else {
                this.callback(null, 'No response text.');
            }
        } finally {
            this.manager.endCall(request.id);
        }
    },
    
    onFailure: function(response) {
        try {
            this.callback(null, 'HTTP failure: ' + response.status + ': ' + response.statusText);
        } finally {
            this.manager.endCall(request.id);
        }
    }
});

var RPCManager = Class.create({
    initialize: function() {
        this.calls = {};
        this.callID = 0;
    },
    
    beginCall: function(name, args, callback) {
        var id = this.callID++;
        this.calls[id] = new RPCCall(id, name, args, callback, this);
    },
    
    endCall: function(id) {
        this.calls.remove(id);        
    }
});

var rpc = new RPCManager();
