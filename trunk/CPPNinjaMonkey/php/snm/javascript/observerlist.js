var ObserverList = Class.create({
    initialize: function() {
        this.observers = [];
    },
    
    observe: function(handler) {
        this.observers.push(handler);
    },
    
    raise: function() {
        this.observers.each(function(observer) {
            observer();
        });
    }
});
