var products = {
    getAll: function (req, res) {
        var allProducts = data; // Spoof a DB call
        res.json(allProducts);
    },
    getOne: function (req, res) {
        var id = req.params.id;
        var product = data[0]; // Spoof a DB call
        res.json(product);
    },
    create: function (req, res) {
        var newProduct = req.body;
        data.push(newProduct); // Spoof a DB call
        res.json(newProduct);
    },
    update: function (req, res) {
        var updateProduct = req.body;
        var id = req.params.id;
        data[id] = updateProduct // Spoof a DB call
        res.json(updateProduct);
    },
    delete: function (req, res) {
        var id = req.params.id;
        data.splice(id, 1) // Spoof a DB call
        res.json(true);
    }
};
var data = [{
    name: 'product 1',
    id: '1'
}, {
        name: 'product 2',
        id: '2'
    }, {
        name: 'product 3',
        id: '3'
    }];
module.exports = products;
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
var products = {

    getAll: function (req, res) {
        var allProducts = data; // Spoof a DB call
        res.json(allProducts);
    },

    getOne: function (req, res) {
        var id = req.params.id;
        var product = data[0]; // Spoof a DB call
        res.json(product);
    },

    create: function (req, res) {
        var newProduct = req.body;
        data.push(newProduct); // Spoof a DB call
        res.json(newProduct);
    },

    update: function (req, res) {
        var updateProduct = req.body;
        var id = req.params.id;
        data[id] = updateProduct // Spoof a DB call
        res.json(updateProduct);
    },

    delete: function (req, res) {
        var id = req.params.id;
        data.splice(id, 1) // Spoof a DB call
        res.json(true);
    }
};

var data = [{
    name: 'product 1',
    id: '1'
}, {
        name: 'product 2',
        id: '2'
    }, {
        name: 'product 3',
        id: '3'
    }];

module.exports = products;