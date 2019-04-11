open ReasonUrql;

type pokemon = {
  id: string,
  number: string,
};

module GetPokemon = [%graphql
  {|
  query pokemon($name: String!) {
    pokemon @bsRecord {
      id
      number
    }
  }
|}
];

let pokemonQuery = GetPokemon.make(~name="pikachu");

let component = ReasonReact.statelessComponent("Example");

let make = (~client: Client.t, _children) => {
  ...component,
  render: _self =>
    <Query query=pokemonQuery>
      <button onClick={executeQuery()}> ExecuteQuery </button>
      {
        switch (queryRenderProps##data) {
        | Some(data) => ReasonReact.string("beep")
        | None => ReasonReact.string("no")
        }
      }
    </Query>,
};