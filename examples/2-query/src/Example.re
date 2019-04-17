open ReasonUrql;

type pokemon = {
  id: string,
  number: string,
};

module GetPokemon = [%graphql
  {|
  query pokemon($name: String!) {
    pokemon(name: $name) {
      id
      number
      image
    }
  }
|}
];
let component = ReasonReact.statelessComponent("Example");

let query = GetPokemon.make(~name="Pikachu", ())##query;
Js.log(GetPokemon.makeWithVariables({"name": "Pikachu"}));

let variables = Js.Dict.empty();
Js.Dict.set(variables, "name", Js.Json.string("Pikachu"));
let payload = Js.Json.object_(variables);
let parse = GetPokemon.make(~name="Pikachu", ())##parse;

let make = _children => {
  ...component,
  render: _self =>
    <Query query variables=payload>
      ...{
           ({response, executeQuery}: Query.queryRenderProps(GetPokemon.t)) =>
             <div>
               {
                 switch (response) {
                 | Data(data) =>
                   switch (data##pokemon) {
                   | Some(pokemon) =>
                     switch (pokemon##image) {
                     | Some(image) => <img src=image />
                     | None => <div />
                     }
                   | None => <div />
                   }
                 | Fetching =>
                   <div> {ReasonReact.string("Henlo I am loading")} </div>
                 | Error(error) =>
                   <div> {ReasonReact.string("OH NO!")} </div>
                 | NotFound =>
                   <div> {ReasonReact.string("You messed up son!")} </div>
                 }
               }
             </div>
         }
    </Query>,
};

/* let pokemonData = data->parse;
   switch (pokemonData##pokemon) {
   | Some(pokemon) =>
     switch (pokemon##image) {
     | Some(image) => <img src=image />
     | None => ()
     } */