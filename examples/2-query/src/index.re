open ReasonUrql;

let client = Client.make(~url="https://graphql-pokemon.now.sh", ());

Js.log(client);

ReactDOMRe.renderToElementWithId(
  <Provider value=client> <Example /> </Provider>,
  "root",
);